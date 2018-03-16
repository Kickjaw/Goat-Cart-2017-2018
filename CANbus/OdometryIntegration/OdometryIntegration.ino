/////INCLUDES/////
#include <TimerOne.h>
#include “FlexCAN.h”

////////////////////SHARED VARIABLES////////////////////
//TEENSY 3.5
const int SENSOR_PIN = 3;           //Pin connected to sensor
int led1 = 28;                      //LED on CAN board

//GOAT CART
const float radius = 20.955;        //Radius of the wheel in cm - USER DEFINED
float circumference;                //Circumference of the wheel (2*pi*radius)

//SENSOR
const int maxDebounceCount = 9;   //Shortest possible revolution in ms (for debouncing) - USER DEFINED
int debounceCount;                  //Counter (for debouncing)
int sensorVal;                      //Value of digital read of SENSOR_PIN (HIGH or LOW)
boolean debounceFlag;               //Flag to indicate state of debounce mode (true or false)
boolean stillLow;

//DATA STORAGE & PROCESSING
volatile int mph;                 //To store speed (in mph)          
const float toMPH = 22.3694;        //Constant to convert cm/ms to mph
float revolutions;                    //To store number of total revolutions
volatile int feet;
const float toFeet = 0.0328084;   //Constant to convert cm to ft
int highForFive;

//TIMER
long revTimeDelta;                  //Time between one full rotation in ms
const int timeout = 2000;           //Maximum time between revolutions in ms - USER DEFINED

//CAN
FlexCAN CANbus(500000);
static CAN_message_t speed_msg, dist_msg;
int txCount;

////////////////////SETUP////////////////////
void setup() 
{
  /////INITIALIZE VAIRABLES/////
  dist_msg.id = 0x54;
  debounceCount = maxDebounceCount;
  mph = 0;
  revTimeDelta = 0;
  highForFive = 0;
  circumference = 2*3.14*radius;
  debounceFlag = false;
  stillLow = false;
  /////CONFIGURE PINS/////
  //Switch Closed - Pin 2 connects directly to ground (LOW)
  //Switch Open - Pin 2 biased via interal pull-up resistor (HIGH)
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  /////TIMER INTERRUPT SETUP/////
  Timer1.initialize(1000);
  Timer1.attachInterrupt(readSensor);
  /////CONFIGURE SERIAL DATA TRANSMISSION/////
  //Baud rate of 9600 bps
  Serial.begin(9600);  
  /////INITIALIZE CANBUS/////   
  CANbus.begin();
  pinMode(led1, OUTPUT);
  digitalWrite(led1, 0);          
}

////////////////////ISR////////////////////
void readSensor()
{
  /////SENSOR///// 
  sensorVal = digitalRead(SENSOR_PIN);                          
  if(debounceFlag == false)
  {
    if(sensorVal == LOW)
    {
      if(stillLow == false)
      {
        debounceFlag = true;
      }
      else if(stillLow == true)
      {
        //Do nothing
      }
    }
    else if(sensorVal == HIGH)
    { 
      if(stillLow == true)
      {
        if(highForFive == 5)
        {
          highForFive = 0;
          stillLow = false;
          revolutions += 0.25;                                           
          feet = circumference*revolutions*toFeet;
          mph = toMPH*((circumference*0.25)/float(revTimeDelta));
          revTimeDelta = 0;
          debounceCount = maxDebounceCount;
        }
        else if(highForFive < 5)
        { 
          highForFive++;
        }
      }
      else if(stillLow == false)
      {
        //Do nothing
      }
    }
  }
  else if(debounceFlag == true)
  {
    if(sensorVal == LOW)
    {
      if(debounceCount == 0)
      {
        debounceFlag = false;
        stillLow = true;
      }
      else if(debounceCount > 0)
      {
        debounceCount--;
      }
    }
    else if(sensorVal == HIGH)
    {
      if(debounceCount > 0)
      {
        debounceCount--;
      }
    }
  }
  /////TIMER/////
  if (revTimeDelta > timeout){                                 
    mph = 0;                                                   
  }
  else{
    revTimeDelta++;                                        
  } 
}

////////////////////HELPER FUNCTIONS////////////////////
void displaySpeed(float mph)
{
  //Print speed
  Serial.print("Speed: ");
  Serial.println(mph, 4);
}

void displayDistance(float feet)
{
  //Print distance
  Serial.print("Distance: ");
  Serial.println(feet, 4);
}

////////////////////LOOP////////////////////
void loop() 
{
  //Local variables copies
  int mphCopy;
  int feetCopy;
  //Disable interrupts, copy critical, time-sensitive code, enable interrupts
  noInterrupts();
  mphCopy = mph;
  feetCopy = feet;
  interrupts();
  int bussize;
  //Determine Buffer Size
  //We want to separate feetCopy into the different 
  //buffer positions if it reaches the position's capacity 
  //First we need to know how many positions it will occupy 
  //a position holds up to 255
  //if msg value is 260 it will need two buffer positions 255-5
  
  if ((2040 > feetCopy)  && (feetCopy >= 256)){
    bussize = (feetCopy/256) + 1;
  }
  else if (feetCopy < 256) {
    bussize = 1;
  }
  else {
    bussize = 8;
  }
  dist_msg.len = bussize;
  
  //Fill Buffer
  int i = 0;
  while (bussize){
    if (feetCopy < 256){
      dist_msg.buf[i] = (uint8_t) feetCopy;
    }
    else {
      dist_msg.buf[i] = 255;
      feetCopy -= 255;
      i++;
    }
     bussize--;
   }
   
  //Send Message to Bus
  txCount = 1;  
  digitalWrite(led1, 1);
  while ( txCount-- ) {
    CANbus.write(dist_msg);
    Serial.println(“msg sent”);  
  }
  digitalWrite(led1, 0);
}

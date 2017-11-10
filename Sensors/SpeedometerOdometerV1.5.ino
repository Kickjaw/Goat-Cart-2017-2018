/////INCLUDES/////
#include <TimerOne.h>

////////////////////SHARED VARIABLES////////////////////
//TEENSY 3.5
const int SENSOR_PIN = 3;           //Pin connected to sensor

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
volatile float mph;                 //To store speed (in mph)          
const float toMPH = 22.3694;        //Constant to convert cm/ms to mph
float revolutions;                    //To store number of total revolutions
volatile float feet;
const float toFeet = 0.0328084;   //Constant to convert cm to ft
int highForFive;

//TIMER
long revTimeDelta;                  //Time between one full rotation in ms
const int timeout = 2000;           //Maximum time between revolutions in ms - USER DEFINED

////////////////////SETUP////////////////////
void setup() 
{
  /////INITIALIZE VAIRABLES/////
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
  Serial.println(mph,4);
}

void displayDistance(float feet)
{
  //Print distance
  Serial.print("Distance: ");
  Serial.println(feet,4);
}

////////////////////LOOP////////////////////
void loop() 
{
  //Local variables copies
  float mphCopy;
  float feetCopy;
  //Disable interrupts, copy critical, time-sensitive code, enable interrupts
  noInterrupts();
  mphCopy = mph;
  feetCopy = feet;
  interrupts();
  //Display speed and distance after every 1000 ms (1 second)
  displaySpeed(mphCopy);
  displayDistance(feetCopy);
  delay(1000);
}

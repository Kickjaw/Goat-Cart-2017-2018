/////INCLUDES/////
#include <TimerOne.h>

////////////////////SHARED VARIABLES////////////////////
//TEENSY 3.5
const int SENSOR_PIN = 3;           //Pin connected to sensor

//GOAT CART
const float radius = 20.955;        //Radius of the wheel in cm - USER DEFINED
float circumference;                //Circumference of the wheel (2*pi*radius)

//SENSOR
const int maxDebounceCount = 20;   //Shortest possible revolution in ms (for debouncing) - USER DEFINED
int debounceCount;                  //Counter (for debouncing)
int sensorVal;                      //Value of digital read of SENSOR_PIN (HIGH or LOW)
boolean debounceFlag;               //Flag to indicate state of debounce mode (true or false)

//DATA STORAGE & PROCESSING
volatile float mph;                 //To store speed (in mph)          
const float toMPH = 22.3694;        //Constant to convert cm/ms to mph
int revolutions;                    //To store number of total revolutions
volatile float feet;
const float toFeet = 0.0328084; //Constant to convert cm to ft

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
  circumference = 2*3.14*radius;
  debounceFlag = false;
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
  sensorVal = digitalRead(SENSOR_PIN);                          //Read value at SENSOR_PIN (HIGH or LOW)
  if(sensorVal == LOW && debounceFlag == false)                 //Step 1: Check to see when SENSOR_PIN transitions from HIGH to LOW
  {
    debounceFlag = true;                                        //Enter debounce state
  }
  else if(sensorVal == LOW && debounceFlag == true) //Step 2: Check status of debounce state
  {
    if(debounceCount == 0)                                      //Check to see if debounce interval has passed
    { 
      revolutions++;                                            //Debounce interval has passed, successful rotation, update variables
      feet = float(circumference)*float(revolutions)*toFeet;
      mph = toMPH*(float(circumference)/float(revTimeDelta));
      revTimeDelta = 0;
      debounceCount = maxDebounceCount;
      debounceFlag = false;
    }
    else if(debounceCount > 0)                                  //Check to see if in debounce interval
    {
      debounceCount--;                                          //Decrement debounce count
    }
  }
  else if(sensorVal == HIGH && debounceFlag == true)            //Step 3: Check to see if in debounce state
  {
    if(debounceCount > 0)                                       //Check to see if in debounce interval
    {
      debounceCount--;                                          //Decrement debounce count
    }
  }
  /////TIMER/////
  if (revTimeDelta > timeout){                                  //Check if maximum time between revolutions has passed  
    mph = 0;                                                    //Cart at standstill, set speed to zero
  }
  else{
    revTimeDelta++;                                             //Increment time since last rotation
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



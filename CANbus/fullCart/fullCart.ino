#include <SPI.h>
#include <Servo.h>
#include <TimerOne.h>

//Odometry 
//TEENSY 3.5
const int SENSOR_PIN = 3;           //Pin connected to sensor

//GOAT CART
const float radius = 20.955;        //Radius of the wheel in cm - USER DEFINED
float circumference;                //Circumference of the wheel (2*pi*radius)
int stop_cart = 0; 

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

//MOTOR 
Servo steerMotor;
Servo brakeMotor;

//pins
const int ledPin = 13;
const int CS = 15;
const int brakePin = 36
;
const int steerPin = 35;

//values
int hard_brake_value = 90;
int hard_steer_value = 90;
int throttle_val;
byte address = 0x00;
int incomingByte; //user input 
int input; //case for switch statment to select which pot to use and what to set the motor to

  float tempFeet = 0; 
  int repeats = 1;

void setup() {
    /////INITIALIZE VAIRABLES/////
  debounceCount = maxDebounceCount;
  mph = 0;
  revTimeDelta = 0;
  highForFive = 0;
  circumference = 2*3.14*radius;
  debounceFlag = false;
  stillLow = false;
  //other stuff 

  /////CONFIGURE PINS/////
  steerMotor.attach(steerPin);
  brakeMotor.attach(brakePin);
  //Switch Closed - Pin 2 connects directly to ground (LOW)
  //Switch Open - Pin 2 biased via interal pull-up resistor (HIGH)
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  /////TIMER INTERRUPT SETUP/////
  Timer1.initialize(1000);
  Timer1.attachInterrupt(readSensor);
  //MOTOR
  pinMode (CS, OUTPUT);
  SPI.begin();
  Serial.begin(9600);
  throttle_val = 0; 
  digitalPotWrite(throttle_val);
}

int digitalPotWrite(int value)
{
  digitalWrite(CS, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS, HIGH);
}

void turnLeft(){ //update these values
  steerMotor.write(65); //turn 1 dir
  delay(100);
  steerMotor.write(90);  //stop
}

void turnRight(){
  steerMotor.write(120); //turn 1 dir
  delay(100);
  steerMotor.write(90);  //stop
}
void brakeOn() {
  brakeMotor.write(135); // turns on one direction 
  delay(1000);
  brakeMotor.write(91); //stop motor 
}

void brakeOff() {
  brakeMotor.write(46); //turn the pther direction 
  delay(750);
  brakeMotor.write(91); //stop 
}

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



void loop(){ 
    //Local variables copies 
    float mphCopy;
    float feetCopy; 
    //Disable interrupts, copy critical, time-sensitive code, enable interrupts
    noInterrupts();
    mphCopy = mph;
    feetCopy = feet;
    interrupts();
    //Display speed and distance after every 1000 ms (1 second)
    //displaySpeed(mphCopy);
    //displayDistance(feetCopy);
    
   
    if (Serial.available() > 0) { 
      incomingByte = Serial.read();
      switch(incomingByte) {
        case 'i': //go forward
          tempFeet = feetCopy;
          int i; 
          brakeOff(); //liberate break 
          //increase velocity three times to get to a relatively slow velocity
          for (i=0; i< 3; i++){ 
            Serial.println(throttle_val);
            if (throttle_val < 0) {
              throttle_val = 0;
            }
            else if (throttle_val >= 118){
              throttle_val = 128;
            }
            else {
              throttle_val += 10; 
            } 
            digitalPotWrite(throttle_val);
            delay(100);
          }
          Serial.println("going forward");
          break;
        case 'k': //stop
          throttle_val = 0;
          brakeOn();
          break;
        case 'j': //turn left
          turnLeft();
          break;
        case 'l': //turn right
          turnRight();
          break;
        case 'u': //brake on
          brakeOn();
          break;
        case 'o': //brake off
          brakeOff();
          break;
        case 'q': //go ten feet forward
          break;
          
        case '1': //granular brake control
          hard_brake_value += 5;
          brakeMotor.write(hard_brake_value);
          break;
        case '2': //granular brake control
          hard_brake_value -= 5;
          brakeMotor.write(hard_brake_value);
          break;
        case '3': //hard brake motor stop
          hard_brake_value = 91;
          brakeMotor.write(hard_brake_value);
          break;
        case '4': //granular steer left
          hard_steer_value -= 5;
          steerMotor.write(hard_steer_value);
          break;
        case '5': //granular steer right
          hard_steer_value += 5;
          steerMotor.write(hard_steer_value);
          break;
        case '6': //hard steer stop
          hard_steer_value = 90;
          steerMotor.write(hard_steer_value);
          break;
        case '7': //hard throttle incrment
          throttle_val += 5;
          digitalPotWrite(throttle_val);
          break;
        case '8': //hard throttle decriment
          throttle_val -= 5;
          digitalPotWrite(throttle_val);
          break;
        case '9': //hard thrttle shut off
          throttle_val = 0;
          digitalPotWrite(throttle_val);
          break;
          
      }
      Serial.print("Hard brake value = ");
      Serial.println(hard_brake_value);
      Serial.print("Hard steer value = ");
      Serial.println(hard_steer_value);
      Serial.print("Throttle value = ");
      Serial.println(throttle_val);
      Serial.println();
  }
}


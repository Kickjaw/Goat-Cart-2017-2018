#include <SPI.h>
#include <Servo.h>

Servo myservo;

//pins
const int ledPin = 13;
const int CS = 15;

int throttle_val;
byte address = 0x00;
int incomingByte; //user input 
int input; //case for switch statment to select which pot to use and what to set the motor to



void setup() {
  pinMode (CS, OUTPUT);
  SPI.begin();
  Serial.begin(9600);
  throttle_val = 128; 
}

int digitalPotWrite(int value)
{
  digitalWrite(CS, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS, HIGH);
}

void brakeOn() {
  myservo.write(135); // turns on one direction 
  delay(1000);
  myservo.write(91); //stop motor 
}

void brakeOff() {
  myservo.write(46); //turn the pther direction 
  delay(750);
  myservo.write(91); //stop 
}

void loop(){ 
   if (Serial.available() > 0) { 
    incomingByte = Serial.read();
    Serial.println(incomingByte);
    if (incomingByte == 'i'){  //whenever it receives command i do the Demo
      Serial.println("in\n");
      int i; 
      brakeOff(); //liberate break 
      //increase velocity three times to get to a relatively slow velocity
      for (i=0; i< 3; i++){ 
        Serial.println(throttle_val);
        if (throttle_val <= 78) {
          throttle_val = 70;
        }
        else {
          throttle_val -= 15; 
        } 
        digitalPotWrite(throttle_val);
        delay(1000);
      }
      Serial.println("wait 8 seconds \n");
      delay(8000); 
      throttle_val = 128;
      digitalPotWrite(throttle_val);
      brakeOn();
      myservo.write(91);
      Serial.println(throttle_val);
      Serial.write('n'); 
    }
  }
}

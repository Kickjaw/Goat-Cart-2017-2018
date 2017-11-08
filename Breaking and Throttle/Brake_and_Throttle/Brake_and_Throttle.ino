/*
Wiring Configuration (Arduino Uno & DS1803-010 IC)
     Arduino GND -> GND Bus (Breadboard / Perf Board)
     Tie A2, A1, A0, and GND to GND (Pins 5, 6, 7, 8 of IC to GND)
     Attach 3.3v Teensy 3.5 (third down on the right side)
     Attach SCL0 Teensy 3.5 (pin 19) to Chip SCL (Pin 9) -- 
     Attach SDA0 Teensy 3.6 (pin 18) to Chip SDA (Pin 10) -- 
     Measuring Resistance across High End of Resistor (H0) and Wiper Terminal of Resistor (W0) (Pins 14 and 12 of IC) for brake control
     Measuring Resistance across High End of Resistor (H1) and Wiper Terminal of Resistor (W1) (Pins 1 and 4 of IC) for throttle control
 
Operation
     Chip Defaults to >10k Ohms on Power-On w/ No I2C Signal
     Send startup values of 128 adn 255 to brake pot and throttle pot respectivly to have them not run on start up
     based on what input the function reiceves (from either canbus or terminal) there are set actions to do
     the brake motor has three states, off, forward, and backward
     throttle has 2 states, off and increase
     
Serial Monitor is Enabled on 9600 Baud Rate for Monitoring Value of Variable "val"
 
*/
 
#include <SPI.h>
#include <Servo.h>

Servo myservo;

//pins
const int ledPin = 13;
const int CS = 15;

int throttle_val = 128;
byte address = 0x00;
int incomingByte; //user input 
int input; //case for switch statment to select which pot to use and what to set the motor to
 
void setup() {
  pinMode (CS, OUTPUT);
  SPI.begin();
  Serial.begin(9600);
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


 
void loop() {  
   digitalPotWrite(throttle_val);
  if (Serial.available() > 0) {
    incomingByte = Serial.read();

    switch(incomingByte) {
      case '1':
        throttle_val = 255;
        myservo.write(91);
        Serial.println("Brake motor killed");
        break;
      case '2':
        throttle_val = 255;
        brakeOn();
        myservo.write(91);
        Serial.println("Brake on");
        break;
      case '3':
        throttle_val = 255;
        brakeOff();
        myservo.write(91);
        Serial.println("Brake off");
      case '4': //throttle off
        throttle_val = 255;
        Serial.println("Throttle off");
        break;
        
      case '5': //throttle increment up
        if (throttle_val < 10) {
          throttle_val = 5;
        }
        else {
          throttle_val -= 10; 
        }
          
        Serial.println("Throttle increment up");
        break;
        
      case '6': //throttle increment down
        if (throttle_val > 118) {
          throttle_val = 124;
        }
        else {
          throttle_val += 10;
        }
        Serial.println("Throttle increment down");
        break;
      default:
        myservo.write(91);
        break;
    }
  Serial.println(throttle_val);
  }
  delay(100);
}




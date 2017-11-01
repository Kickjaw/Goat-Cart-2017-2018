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
 
#include <Wire.h>
 
byte throttle_val = 255;
byte brake_val = 128;
byte bothPots = B10101011; //bytes to select both pots
int incomingByte; //user input 
int input; //case for switch statment to select which pot to use and what to set the motor to
 
void setup() {
  Wire.begin();
  Serial.begin(9600);
  Wire.beginTransmission(0x28);
  Wire.write(bothPots); //start up values so motors dont run on startup
  Wire.write(brake_val);
  Wire.write(throttle_val);
  Wire.endTransmission();
}
 
void loop() {
  
  Wire.beginTransmission(0x28);
  Wire.write(bothPots);
  Wire.write(brake_val);
  Wire.write(throttle_val);
  Wire.endTransmission();
 
  if (Serial.available() > 0) {

    incomingByte = Serial.read();

    input = incomingByte;
    Serial.println(incomingByte);
    switch(input) {
      case '1': //brake motor off
        brake_val = 128;
        throttle_val = 255;
        Serial.println("Brake motor off");
        break;
        
      case '2': //brake motor full forward
        brake_val = 0;
        throttle_val = 255;
        Serial.println("Brake full forward");
        break;
        
      case '3': //brake motor full reverse
        brake_val = 255;
        throttle_val = 255;
        Serial.println("Brake full reverse");
        break;
        
      case '4': //throttle off
        brake_val = 128;
        throttle_val = 255;
        Serial.println("Throttle off");
        break;
        
      case '5': //throttle increment up
        brake_val = 128;
        throttle_val += 10;
        Serial.println("Throttle increment up");
        break;
        
      case '6': //throttle increment down
        brake_val = 128;
        throttle_val -= 10;
        Serial.println("Throttle increment down");
        break;
    }
    delay(100);
   }//end if serial avail
  delay(100);
}


/*
Wiring Configuration (Arduino Uno & DS1803-010 IC)
     Arduino GND -> GND Bus (Breadboard / Perf Board)
     Tie A2, A1, A0, and GND to GND (Pins 5, 6, 7, 8 of IC to GND)
     Attach Arduino 5V to Chip VCC (Pin 16) -- 3.3v Teensy 3.6 (third down on the right side)
     Attach Arduino Analog 5 (SCLK) to Chip SCL (Pin 9) -- SCL0 Teensy 3.6 (pin 19)
     Attach Arduino Analog 4 (SDA) to Chip SDA (Pin 10) -- SDA0 Teensy 3.6 (pin 18)
     Measuring Resistance across High End of Resistor (H0) and Wiper Terminal of Resistor (W0) (Pins 14 and 12 of IC)
 
Operation
     Chip Defaults to >10k Ohms on Power-On w/ No I2C Signal
     Val Initiates to 142, which is 5k Ohms - Full Stop on Golf Cart Accelerator
     /Val Increments from 142 -> 255, which decrements the resistance proportionately from 5000 Ohms -> 404.5
     At Val = 255, it jumps to Val = 142, which is Full Speed back to Complete Stop
     Cycle Loops Indefinitely
 
Serial Monitor is Enabled on 9600 Baud Rate for Monitoring Value of Variable "val"
 
*/
 
#include <Wire.h>
 
byte throttle_val = 255;
byte brake_val = 128;
int incomingByte; //user input 
int input; //case for switch statment to select which pot to use and what to set the motor to
 
void setup() {
  Wire.begin();
  Serial.begin(9600);
}
 
void loop() {
  
  Wire.beginTransmission(0x28);
  Wire.write(B10101001);
  Wire.write(brake_val);
  Wire.write(throttle_val);
  Wire.endTransmission();
 
  if (Serial.available() > 0) {

    incomingByte = Serial.read();
    
    Serial.println(incomingByte);
    switch(incomingByte) {
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

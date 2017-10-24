/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

#include <SPI.h>

byte address = 0x00;
int CS = 10;
int val = 64; //range of 0 - 128
             // values < 64 means motor in reverse
             // values > 64 means motor in forward
int incomingByte;             

void setup() {
  pinMode (CS, OUTPUT);
  SPI.begin();
  Serial.begin(9600);
}
  
int digitalPotWrite(int value) {
  digitalWrite(CS, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS, HIGH);
}
void loop() {
  if (Serial.available() > 0) {
    
     incomingByte = Serial.read();

     if (incomingByte == '+') {
      val += 4;
     }
     if (incomingByte == '-') {
      val -= 4;
     }

     if (incomingByte == 'k') {
      val = 64;
     }

     if (val > 128) {
      val = 128;
     }
  }
  digitalPotWrite(val);
  Serial.println(val);
  delay(100);
}


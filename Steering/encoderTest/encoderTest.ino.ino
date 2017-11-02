/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>

//Pins
Encoder steerEnc(22, 23);
const int ledPin = 13;

//global Vars
long currPos  = -999;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  Serial.println("Encoder Test:");
}



void loop() {
  digitalWrite(ledPin, LOW);
  
  long newPos;
  newPos = steerEnc.read();
  if (newPos != currPos ) {
    digitalWrite(ledPin, HIGH);
    Serial.print("Position = ");
    Serial.print(newPos);
    Serial.println();
    currPos = newPos;
  }
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    digitalWrite(ledPin, HIGH);
    Serial.read();
    Serial.println("Reset Enc");
    steerEnc.write(0);
    Serial.println(steerEnc.read());
  }
}

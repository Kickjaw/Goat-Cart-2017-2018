#include <Servo.h>
Servo myservo;

//pins
const int ledPin = 13;
const int steerPin = 35;

//global vars
int steerSpeed = 90; //no move
int incomingByte = 0;

void setup () {
  myservo.attach(steerPin); // Use PWM pin 14 to control Sabertooth.
}

//93-96 stoped
//96 clockwise


void loop() {  
if (Serial.available() > 0) {
    incomingByte = Serial.read();
    switch(incomingByte) {
      case '1':
        steerSpeed += 1;
        break;
       case '2':
        steerSpeed -= 1;
        break;
     }
     delay(10);
  }
  Serial.println(steerSpeed);
  myservo.write(steerSpeed);
}
  
 

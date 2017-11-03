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

void loop() {  
if (Serial.available() > 0) {
     incomingByte = Serial.read();
     //stop
     if (incomingByte == '1' && steerSpeed < 180) {
      steerSpeed += 5;     

     }
     //full
     else if(incomingByte == '2' && steerSpeed > 0){
      steerSpeed -= 5;
     }
     else if(incomingByte == '0'){
      steerSpeed = 90;
      Serial.println("stop");
      
     }
     Serial.println(steerSpeed);
     myservo.write(steerSpeed);
     digitalWrite(ledPin, HIGH);
     delay(10);
  }
}
  
 
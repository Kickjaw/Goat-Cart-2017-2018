#include <Servo.h>
Servo myservo;
Servo brakemotor;

//pins
const int ledPin = 13;
const int steerPin = 36;
const int brakePin = 35;

//123R
//73L

//global vars
int steerSpeed = 90; //no move
int incomingByte = 0;

void setup () {
  myservo.attach(steerPin); // Use PWM pin 14 to control Sabertooth.

  myservo.write(90);
}


//93-96 stoped
//96 clockwise


void loop() {  
if (Serial.available() > 0) {
     incomingByte = Serial.read();
     //racck go right
     if (incomingByte == '1' && steerSpeed < 180) {
      steerSpeed = 100;     

     }
     //rack go left
     else if(incomingByte == '2' && steerSpeed > 0){
      steerSpeed = 85;
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
  
 

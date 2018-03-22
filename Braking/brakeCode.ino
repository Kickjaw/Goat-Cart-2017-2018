
#include <Servo.h>

//global setup
Servo brakeMotor;
const int ledPin = 13;
const int brakePin = 37;
const int limitOff = 33; 
const int limitOn = 34; 


//global vars
bool ledState = HIGH;
int brakeSpeed = 90; // no move
int incomingByte = 0;

void setup() {
  Serial.begin(9600);
  pinMode(limitOn, INPUT_PULLUP); //limit switch R
  pinMode(limitOff, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  steerMotor.attach(brakePin);
  steerMotor.write(brakeSpeed);
  
  
}

void loop(){

if (Serial.available() > 0) {
     incomingByte = Serial.read();
     delay(10);
     if (incomingByte == '1' ) {
      brakeOn();   
     }

     else if(incomingByte == '2'){
      brakeOff();
     }
     else if(incomingByte == '0'){
      steerMotor.write(90);// stop 
      Serial.println("Estop");
      
     }
  }
  
}

void brakeOn() {
    
    Serial.println("Brake On");
  while(digitalRead(limitOn) == LOW){
    brakeMotor.write(135);// steerMotor go R 
    
    ledState = !ledState;
    digitalWrite(ledPin, ledState); 
  }
  brakeMotor.write(91);// stop steerMotor
  digitalWrite(ledPin, HIGH);
  Serial.println("limitOn pressed!!!");
  
  
  brakeMotor.write(135); // turns on one direction 
  delay(1000);
  brakeMotor.write(91); //stop motor 
}



void brakeOff(){
  
  Serial.println("Brake Off");
  
  while(digitalRead(limitOff) == LOW){
    brakeMotor.write(46);// steerMotor go R 
    
    ledState = !ledState;
    digitalWrite(ledPin, ledState); 
  }
  brakeMotor.write(91);// stop steerMotor
  digitalWrite(ledPin, HIGH);
  Serial.println("limitOff pressed!!!");
  
}







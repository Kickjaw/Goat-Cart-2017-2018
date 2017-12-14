
#include <Servo.h>

//global setup
Servo steerMotor;
const int ledPin = 13;
const int steerPin = 36;
const int limitR = 33; 
const int limitL = 34; 


//global vars
bool ledState = HIGH;
int steerSpeed = 90; // no move
int incomingByte = 0;

void setup() {
  Serial.begin(9600);
  pinMode(limitR, INPUT); //limit switch R
  pinMode(limitL, INPUT);
  pinMode(ledPin, OUTPUT);
  steerMotor.attach(steerPin);
  steerMotor.write(steerSpeed);
  
  
}

void loop(){

if (Serial.available() > 0) {
     incomingByte = Serial.read();
     //racck go right
     delay(10);
     if (incomingByte == '1' ) {
      goR();   
     }
     //rack go left
     else if(incomingByte == '2'){
      goL();
     }
     else if(incomingByte == '0'){
      steerMotor.write(90);// stop steerMotor
      Serial.println("Estop");
      
     }
  }
  
}

void goR(){
  
  Serial.println("Going R");
  
  while(digitalRead(limitR) == LOW){
    steerMotor.write(110);// steerMotor go R 
    
    ledState = !ledState;
    digitalWrite(ledPin, ledState); 
  }
  steerMotor.write(90);// stop steerMotor
  digitalWrite(ledPin, HIGH);
  Serial.println("Button R pressed!!!");
  
}

void goL(){
  Serial.println("Going L");
  while(digitalRead(limitL) == LOW){
    steerMotor.write(80);// steerMotor go L  

    ledState = !ledState;
    digitalWrite(ledPin, ledState); 
  }
  steerMotor.write(90);// stop steerMotor
  digitalWrite(ledPin, HIGH);  
  Serial.println("Button L pressed!!!");

}





/*
*this is the steering calebration code
* drives steering to the R till hits the limit switch takes the far R val
* drives steering to the L till hits the limit switch takes the far L val
* then takes average and moves to that value and resets it to 0
* 
 */
 
#include <Encoder.h>
#include <Servo.h>

//global setup
Encoder steerEnc(22, 23);
Servo steerMotor;
const int ledPin = 13;
const int steerPin = 36;
const int limitR = 33; 
const int limitL = 34; 

//global vars
bool ledState = HIGH;
long currPos  = -999;
long maxR = 0;
long maxL = 0;
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
     else if(incomingByte == '3'){
      //run
        goFullR();
        goFullL();
        goMid(); 
     }
     else if(incomingByte == '0'){
      steerMotor.write(90);// stop steerMotor
      Serial.println("stop");
      
     }
  }
  
  
}

void goR()
{
  if(digitalRead(limitR) == LOW)
  {
    steerMotor.write(110);// steerMotor go R 
  }
}


void goFullR(){
  Serial.println("Going R");
  
  while(digitalRead(limitR) == LOW){
    steerMotor.write(110);// steerMotor go R 
    
    ledState = !ledState;
    digitalWrite(ledPin, ledState); 
  }
  steerMotor.write(90);// stop steerMotor
  digitalWrite(ledPin, HIGH);
  maxR = steerEnc.read(); //get maxR encoder val

  Serial.println("Button R pressed!!!");
  Serial.print("maxR: ");
  Serial.println(maxR);
}

void goL()
{
  if(digitalRead(limitL) == LOW)
  {
    steerMotor.write(80);// steerMotor go R 
  }
}

void goFullL(){
  Serial.println("Going L");
  while(digitalRead(limitL) == LOW){
    steerMotor.write(80);// steerMotor go L  

    ledState = !ledState;
    digitalWrite(ledPin, ledState); 
  }
  steerMotor.write(90);// stop steerMotor
  digitalWrite(ledPin, HIGH);  
  maxL = steerEnc.read(); //get maxL encoder val

  Serial.println("Button L pressed!!!");
  Serial.print("maxL: ");
  Serial.println(maxL);
}

void goMid(){
  int mid = 0;
  mid = maxR + maxL;
  mid = mid/2;
  //I beleive it be less then
  while((currPos <= mid -1000)||(currPos >= mid + 1000)){
    steerMotor.write(110);// steerMotor go R
    delay(10);

    //this logic is to get to the middle via encoder vals
    long newPos;
    newPos = steerEnc.read();
    if (newPos != currPos ) {
      Serial.print("newPos");
      Serial.println(newPos);
      digitalWrite(ledPin, HIGH);
      currPos = newPos;
    }
  }
  //at the middle
  steerEnc.write(0);
  steerMotor.write(90);
  
  Serial.println("set enc to 0");
  maxR = mid;
  maxL = -mid;
}


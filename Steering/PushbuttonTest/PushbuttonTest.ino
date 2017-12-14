/* Pushbutton, Teensyduino Tutorial #3
   http://www.pjrc.com/teensy/tutorial3.html

   This example code is in the public domain.
*/

const int ledPin = 13;
const int limit1Pin = 33;//left
const int limit2Pin = 34;//right
bool ledState = HIGH;

void setup() {                
  Serial.begin(9600);
  pinMode(limit1Pin, INPUT);
  pinMode(limit2Pin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop()                     
{
  ledState = !ledState;
  digitalWrite(ledPin, ledState); 
  if (digitalRead(limit1Pin) == LOW) {
    Serial.println("Button1 is not pressed...");
  } else if(digitalRead(limit1Pin) == HIGH){
    Serial.println("Button1 pressed!!!");
  }

  if (digitalRead(limit2Pin) == LOW) {
    Serial.println("Button2 is not pressed...");
  } else if(digitalRead(limit2Pin) == HIGH){
    Serial.println("Button2 pressed!!!");
  }
  else{
    Serial.println("shit!");
  }
  delay(400);
}


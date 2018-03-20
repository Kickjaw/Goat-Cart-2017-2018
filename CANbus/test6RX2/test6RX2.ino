//represents the Steering  Teensy, which takes the messages containing commands that are affecting steering  


#include "FlexCAN.h"

int led2 = 28;
int led3 = 29; 
FlexCAN CANbus(500000);
static CAN_message_t msg,rxmsg;
int txCount,rxCount;
int counter; 


void setup(void)
{
  CANbus.begin();
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  digitalWrite(led2, 0);
  digitalWrite(led3, 0);
  counter = 1; 
  msg.id = 0x12; 
  msg.len = 1; 
  msg.buf[0] = 1; 
}


// -------------------------------------------------------------
void loop(void)
{    
   while (CANbus.read(rxmsg) ) {
     if (rxmsg.id == 0x23){ 
      Serial.println("calibrate");
      digitalWrite(led2, 1);
      digitalWrite(led3, 0);
    }
    else if (rxmsg.id == 0x24){ 
      Serial.println("Speed Increase rec");
      digitalWrite(led2, 0);
      digitalWrite(led3, 1);
      counter++; 
    }
    else{
      Serial.println("nothing"); 
    }
    
   }
   if (counter%5 == 0){
        txCount = 1; 
        while ( txCount-- ) {
        CANbus.write(msg);
        Serial.println("Sent Update"); 
        counter++;     
        }
   }
}

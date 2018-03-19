//represents the Steering  Teensy, which takes the messages containing commands that are affecting steering  


#include "FlexCAN.h"

int led2 = 28;
int led3 = 29; 
FlexCAN CANbus(500000);
static CAN_message_t msg,rxmsg;
int txCount,rxCount;



void setup(void)
{
  CANbus.begin();
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  digitalWrite(led2, 0);
  digitalWrite(led3, 0);
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
    }
    else{
      Serial.println("nothing"); 
    }
    
   }
}

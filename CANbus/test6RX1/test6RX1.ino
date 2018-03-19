//represents the Throttle/Breaks  Teensy
//Accepts or takes the messages containing commands that are affecting throttle and breaks
#include "FlexCAN.h"

int led0 = 28;
int led1 = 29; 
FlexCAN CANbus(500000);
static CAN_message_t msg,rxmsg;
int txCount,rxCount;



void setup(void)
{
  CANbus.begin();
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  digitalWrite(led0, 0);
  digitalWrite(led1, 0);
}


// -------------------------------------------------------------
void loop(void)
{    
   while (CANbus.read(rxmsg) ) {
     if (rxmsg.id == 0x0A){ 
      Serial.println("Breaks On rec");
      digitalWrite(led0, 1);
      digitalWrite(led1, 0);
    }
    else if (rxmsg.id == 0x14){ 
      Serial.println("Speed Increase rec");
      digitalWrite(led0, 0);
      digitalWrite(led1, 1);
    }
    else{
      Serial.println("nothing"); 
    }
  }
}


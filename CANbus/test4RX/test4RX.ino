//This test is about lighing up a specific LED depending on the Msg received
//It also states how to receive and trasmitt Steering commands. 

#include "FlexCAN.h"

int led1 = 28;
int led2 = 29; 
FlexCAN CANbus(500000);
static CAN_message_t msg,rxmsg;
int txCount,rxCount;



void setup(void)
{
  CANbus.begin();
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, 0);
  digitalWrite(led2, 0);
}


// -------------------------------------------------------------
void loop(void)
{
    while (CANbus.read(rxmsg) ) {
     if (rxmsg.id == 0x05){ 
      digitalWrite(led1, 1);
      delay (1000); 
      Serial.println((char)rxmsg.buf[0]);
      Serial.println((int)rxmsg.buf[1]);
      digitalWrite(led1, 0);
    }
    else if(rxmsg.id == 0x20){
      digitalWrite(led2, 1);
      delay (1000); 
      Serial.println((char)rxmsg.buf[0]);
      Serial.println((int)rxmsg.buf[1]);
      digitalWrite(led2, 0);
    }
    else{
      Serial.println("MSG ID NOT RECOGNIZED"); 
    }

  }
}

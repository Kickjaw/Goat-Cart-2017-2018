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
  msg.len = 1; 
  msg.id = 0x06;
  msg.buf[0] = 'a';  
}


// -------------------------------------------------------------
void loop(void)
{    
   while (CANbus.read(rxmsg) ) {
     if (rxmsg.id == 0x05){ 
      Serial.println("msg rec");
      digitalWrite(led1, 1);
      delay (1000); 
      Serial.println((char)rxmsg.buf[0]);
      Serial.println((int)rxmsg.buf[1]);
      digitalWrite(led1, 0);
    }
   }

    txCount = 6; 
//  while ( txCount-- ) {
//    CANbus.write(msg);
//    Serial.println("msg sent");
//
//  }
}


#include "FlexCAN.h"

int led = 28;
FlexCAN CANbus(500000);
static CAN_message_t msg,rxmsg;
int txCount,rxCount;
unsigned int txTimer,rxTimer;

void setup(void)
{
  CANbus.begin();
  pinMode(led, OUTPUT);
  pinMode(32, OUTPUT);
  digitalWrite(led, 0);
}

void loop(void)
{
  msg.id = 0x54; 
  msg.len = 1; 
  msg.buf[0] = 0; 
  txCount = 6; 
  while ( txCount-- ) {
    CANbus.write(msg);
  }
}



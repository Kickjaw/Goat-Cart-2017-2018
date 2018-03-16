#include "FlexCAN.h"

int led = 28;
FlexCAN CANbus(500000);
static CAN_message_t msg,rxmsg;
int txCount,rxCount;

void setup(void)
{
  CANbus.begin();
  pinMode(led, OUTPUT);
}

void loop(void)
{
  while (CANbus.read(rxmsg) ) {
    if (rxmsg.id == 0x54){
      digitalWrite(led, 1);
      Serial.println(rxmsg.buf[0]);
      digitalWrite(led, 0);
    }
  }
}

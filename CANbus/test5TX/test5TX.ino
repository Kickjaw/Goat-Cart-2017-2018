#include "FlexCAN.h"

int led1 = 28;
int led2 = 29; 

FlexCAN CANbus(500000);
static CAN_message_t msg1, msg2,rxmsg;
int txCount,rxCount;

void setup(void)
{
  CANbus.begin();
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, 0);
  digitalWrite(led2, 0);
 
  msg1.len = 2; 
  msg2.len = 2; 
  msg1.id = 0x05; 
  msg2.id = 0x20; 
  msg1.buf[0] = 'N'; 
  msg1.buf[1] = '1'; 
  msg2.buf[0] = 'P'; 
  msg2.buf[1] = '9';  
}


// -------------------------------------------------------------
void loop(void)
{
// while (CANbus.read(rxmsg) ) {
//  if (rxmsg.id == 0x06){
//    Serial.println("msg 0x06 received");
//    digitalWrite(led1, 1);
//    delay (1000); 
//    digitalWrite(led1, 0);
//  }
//  else if (rxmsg.id == 0x07){
//    digitalWrite(led2, 1);
//    delay (1000); 
//    digitalWrite(led2, 1);
//  }
// }
  
  txCount = 6; 
  while ( txCount-- ) {
    CANbus.write(msg1);
    Serial.println("msg1 sent");
  }

    txCount = 6; 
  while ( txCount-- ) {
    CANbus.write(msg2);
    Serial.println("msg2 sent");
  }


}

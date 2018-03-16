//this test transmitts two different Steering messages and the receiving end 
//is going to decode this messages and light up a specific LED depending on the message

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
  //Send first message 6 times 
  txCount = 6; 
  digitalWrite(led1, 1);   
  delay(1000); 
  while ( txCount-- ) {
    CANbus.write(msg1);
    Serial.println("msg1 sent");
  }
  digitalWrite(led1, 0);

  //Send second message 6 times 
  txCount = 6; 
  digitalWrite(led2, 1);   
  delay(1000); 
  while ( txCount-- ) {
    CANbus.write(msg2);
    Serial.println("msg2 sent");
  }
  digitalWrite(led2, 0);
}

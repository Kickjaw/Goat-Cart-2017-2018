//This test is a program that receives the distance traveled. 
//The program calculates the distance by adding to a total variable 
//the numbers in each of the buffer position  
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
      int total = 0;
      if (rxmsg.id == 0x54){ 
        digitalWrite(led1, 1);
        int i;  
        for (i =0; i< rxmsg.len; i++) {
          total += rxmsg.buf[i]; 
        }
      }
      Serial.println("Feet traveled");
      Serial.println(total);
  }
}

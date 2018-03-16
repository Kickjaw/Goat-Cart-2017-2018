//This test demonstrates how to use the 
//buffer to send the distance traveled
#include "FlexCAN.h"

int led1 = 28;
int led2 = 29; 

FlexCAN CANbus(500000);
static CAN_message_t msg;
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
    int distance_traveled = 400; 
    int bussize; 
    if ((2040 > distance_traveled)  && (distance_traveled >= 256)){ 
      bussize = (distance_traveled/256) + 1; 
    }
    else if (distance_traveled < 256) { 
      bussize = 1; 
    }

    else {
      bussize = 8; 
    }
    
    msg.len = bussize;
    msg.id = 0x54;

    int i = 0; 
    while (bussize){ 
      if (distance_traveled < 256){ 
        msg.buf[i] = (uint8_t) distance_traveled;
      }
      else { 
        msg.buf[i] = 255;
        distance_traveled -= 255; 
        i++; 
      }
      bussize--; 
    }
   
  txCount = 1;  
  digitalWrite(led1, 1);  
  while ( txCount-- ) {
    CANbus.write(msg);
    Serial.println("msg sent");  
  }
  digitalWrite(led1, 0);

}

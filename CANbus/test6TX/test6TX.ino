//Code that reads commands from serial ports and sends to CAN bus the commands


#include "FlexCAN.h"

FlexCAN CANbus(500000);

static CAN_message_t directions, calibrate_steering,center_steering, breaks_on, speed_increase, slow_down, odometry;
int txCount,rxCount;
int led1 = 28;
int led2 = 29; 
int incomingCommand; 


void setup(void)
{
  Serial.println(F("Hello Teensy 3.5 CAN Test."));
  Serial.begin(9600); 
  CANbus.begin();
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, 0);
  digitalWrite(led2, 0);

  //Calibration command 
  calibrate_steering.len = 1; 
  calibrate_steering.id = 0x23; 
  calibrate_steering.buf[0] = 0; 

  //Center Steering command
  center_steering.len = 1; 
  center_steering.id = 0x24;
  center_steering.buf[0] = 0; 

  //Breaks on command 
  breaks_on.len = 1; 
  breaks_on.id = 0x0A;
  breaks_on.buf[0] = 0;  
  
  //speed_increase command
  speed_increase.len = 1; 
  speed_increase.id = 0x14;
  speed_increase.buf[0] = 0; 
}



void loop(void)
{
//    while ( CANbus.read(rxmsg) ) {
//      Serial.println("READ::::");
//      int i; 
//      uint8_t distance_traveled = 0; 
//      for (i=0; i < rxmsg.len; i++){
//        distance_traveled += rxmsg[i];
//      }
//      //hexDump( sizeof(rxmsg), (uint8_t *)&rxmsg );
//     Serial.write(distance_traveled);
//    }

     if (Serial.available() > 0) {
      incomingCommand = Serial.read();
       if (incomingCommand == 'i'){ //go forward
        txCount = 5; 
        while ( txCount-- ) {
        CANbus.write(speed_increase);
        Serial.println("speed_increase sent");     
        } 
       }
       else if (incomingCommand == 'm'){ //breaks on
        txCount = 5; 
        while ( txCount-- ) {
        CANbus.write(breaks_on);
        Serial.println("breaks_on sent");     
        }     
       }
       else if (incomingCommand == 'c'){
        txCount = 5; 
        while ( txCount-- ) {
        CANbus.write(center_steering);
        Serial.println("center_steering sent");     
        } 
       } 
       else if (incomingCommand == 'k'){
        txCount = 5; 
        while ( txCount-- ) {
        CANbus.write(calibrate_steering);
        Serial.println("calibrate_steering sent");     
        }
        }    
     }
}

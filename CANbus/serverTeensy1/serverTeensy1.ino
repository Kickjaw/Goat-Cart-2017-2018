
#include <FlexCAN.h>

//led pin number 
int led = 13;
int stop_cart = 0; 
int move_cart = 1; 
int turn_angle = 20; 
int counter = 2; 


//speed related messages 
#define ID_SPEED_INCREASE 0x14
#define ID_BREAKS_APPLIED 0x0A

//steering related messages 
#define ID_STEERING_TURN_RIGHT 0x25
#define ID_STEERING_TURN_LEFT 0x026
#define ID_STEERING_GO_STRAIGHT 0x024
#define ID_STEERING_SET_UP 0x23

//odometry related messages
#define ID_DISTANCE_TRAVELED 0x09




FlexCAN CANbus(500000);                  //set CANbus baud rate to 500000
static uint8_t hex[17] = "0123456789abcdef";

static CAN_message_t rxmsg;  //message to receive

//messages/commands to send
static CAN_message_t speed_increase; 
speed_increase.id = ID_SPEED_INCREASE;
//speed_increase.len = 1; 
//speed_increase.buf = 0x0001; 
static CAN_message_t brakes_applied; 
brakes_applied.id = ID_BREAKS_APPLIED;
//brakes_applied.len = 1; 
//brakes_applied.buf = 0x0001; 
static CAN_message_t turn_right; 
turn_right.id = ID_STEERING_TURN_RIGHT;
//turn_right.len = 1; 
//turn_right.buf = 0x0001; 
static CAN_message_t turn_left; 
turn_left.id = ID_STEERING_TURN_LEFT;
//turn_left.len = 1; 
//turn_left.buf = 0x0001; 
static CAN_message_t steering_straight; 
steering_straight.id = ID_STEERING_GO_STRAIGHT;
//steering_straight.len = 1; 
//steering_straight.buf = 0x0001; 
static CAN_message_t steering_setup; 
steering_straight.id = ID_STEERING_SET_UP;
//steering_straight.len = 1; 
//steering_straight.buf = 0x0001; 



/*Hexdump function*/

static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t working;
  while( dumpLen-- ) {
    working = *bytePtr++;
    Serial.write( hex[ working>>4 ] );
    Serial.write( hex[ working&15 ] );
  }
  Serial.write('\r');
  Serial.write('\n');
}


/*SetUp function*/

void setup(void)
{
  CANbus.begin();
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);

  delay(1000);
  Serial.println(F("Hello Teensy 3.5 CAN Test."));
  CANbus.write(brakes_applied); //send command to be sure breaks are applied and throttle off 
  CANbus.write(steering_straight); //send command to make sure steering is straight 
 // sysTimer.reset();
}



/*Loop function*/

void loop(void)
{
  while(counter> 0){ 
    if(move_cart) {  
     //do not send more messages after reaching the desired speed
     move_cart = 0; 
    CANbus.write(speed_increase); //send command to start moving the cart forfard  
    CANbus.write(speed_increase); //go to second velocity 
    }

    //while there is at least one receive frame waiting -- constantly receiving odometry   
      if (CANbus.available())  {
        CANbus.read(rxmsg);
        //check that received frame was 
        if (rxmsg.id == ID_DISTANCE_TRAVELED){ 
          Serial.print("CAN bus: "); hexDump(rxmsg.len, rxmsg.buf);
          //check if it reached the distance to stop (DEMO) 
          //note: I am putting a range just in case it never gets the exact number
          if (((rxmsg.buf[0] >= 5) && (rxmsg.buf[0] <= 7)) || ((rxmsg.buf[0] >= 14)){ 
            stop_cart = 1;  
          }
        }
    }

    //check if the distance limit of going forward was met 
    if(stop_cart){ 
      CANbus.write(brakes_applied);
      counter--; 
      stop_cart = 0; 
      while(turn_angle){
        delay(1000);  
        CANbus.write(turn_left);
        turn_angle--; 
      }
      delay(60000); 
      move_cart = 1;  
    }
  }
}


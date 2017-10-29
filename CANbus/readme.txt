Following the FlexCAN.h library (which we are going to use for this MQP) the CAN Frame is going to be constructed by using this structure:

typedef struct CAN_message_t {
  uint32_t id; // can identifier
  uint8_t ext; // identifier is extended
  uint8_t len; // length of data
  uint16_t timeout; // milliseconds, zero will disable waiting
  uint8_t buf[8];
} CAN_message_t;

Intially we are going to have 4 Teensy3.5 communicating through the bus. This are going to be called:
1. Server
2. Speed
3. Steering
4. Odometry

We are going to construct a Library of different message IDs. This message IDs are going to represent either the flag to receive some information related to the msgID from a Teensy, or the msgID itself is going to represent a specific action that some specific Teensy needs to take.

The msgIDs are the following:
- SPEED_INCREASE(20) 0x14: When received by the Speed Teensy, increase Digital Potentiometer Value +4. Create condition if the value gets to the maximum speed we want.
- BREAKS_APPLIED(10) 0x0A: More logic is going to be added in a future, but for now it will set speed to 0 and turn off the throttle
- STEERING_TURN_RIGHT(37) 0x25: Receiving this message will make the steering wheel to turn 1 degree to the right
- STEERING_TURN_LEFT(38) 0x026: Receiving this message will make the steering wheel to turn 1 degree to the left
- STEERING_GO_STRAIGHT(36) 0x024: Receiving this message will make the steering wheel to decrease or increase 1 degree until it reaches the middle position
- DISTANCE_TRAVELED(9) 0x09: Send out value about how many feet has the GolfCart traveled

A general outline of how each Teensy will be programmed:
1. Server:
This Teensy will send messages (ID#: 0x14, 0x0A, 0x25, 0x26, 0x24) to the Speed and Steering Teensy about commands this microcontrollers should follow. It will be receiving at the same time frames from the Odometry (ID#: 0x9) to constantly update the distance traveled and use this information to decide what command is going to be sent (move forward, stop, turn right).
2. Speed:
This Teensy for the moments is just going to be processing frames with ID# 0x14 and 0x0A. Will not send anything unless we want to send an ERROR flag that makes the autonomous vehicle to disengage.
3. Steering:
This Teensy for the moments is just going to be processing frames with ID# 0x25, 0x26 and 0x24. Will not send anything unless we want to send an ERROR flag that makes the autonomous vehicle to disengage.
4. Odometry:
This Teensy will not be processing any frames from now. Will just be sending a frame containing total miles travelled (ID# 0x09)
For now, this is the only frame that will make use of the frame body.

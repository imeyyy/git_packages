/* 
 * rosserial Subscriber Example
 * Blinks an LED on callback
 */

#include <ros.h>
#include <std_msgs/String.h>
#include <irobot/cmd_move.h>
#include <irobot/cmd_rotate.h>
#include <SoftwareSerial.h>

 //* RX is digital pin 10 (connect to TX of other device - iRobot DB25 pin 2)
 //* TX is digital pin 11 (connect to RX of other device - iRobot DB25 pin 1)
#define rxPin 10
#define txPin 11

#define BITS_IN_BYTE 8
#define MSB_BYTE_BITS 0xFF00
#define LSB_BYTE_BITS 0x00FF

// set up a new software serial port:
SoftwareSerial softSerial =  SoftwareSerial(rxPin, txPin);

ros::NodeHandle  nh;

void moveCmdCb(const irobot::cmd_move& move_cmd){
  String robot_move_cmd = move_cmd.cmd_move_str;
  if (robot_move_cmd == "forward" || robot_move_cmd == "back") {
    moveRobot((byte)((move_cmd.velocity   & MSB_BYTE_BITS) >> BITS_IN_BYTE), (byte)(move_cmd.velocity   & LSB_BYTE_BITS),
              (byte)((move_cmd.radius     & MSB_BYTE_BITS) >> BITS_IN_BYTE), (byte)(move_cmd.radius     & LSB_BYTE_BITS),
              (byte)((move_cmd.distance   & MSB_BYTE_BITS) >> BITS_IN_BYTE), (byte)(move_cmd.distance   & LSB_BYTE_BITS));
  }
}

void rotateCmdCb(const irobot::cmd_rotate& rotate_cmd){
  String robot_rotate_cmd = rotate_cmd.cmd_rotate_str;
  if (robot_rotate_cmd == "right") {
    rotateRobot((byte)((rotate_cmd.velocity & MSB_BYTE_BITS) >> BITS_IN_BYTE), (byte)(rotate_cmd.velocity & LSB_BYTE_BITS),
                (byte)(0xFF),                                                  (byte)(0xFF),
                (byte)((rotate_cmd.angle    & MSB_BYTE_BITS) >> BITS_IN_BYTE), (byte)(rotate_cmd.angle    & LSB_BYTE_BITS));
  } else if (robot_rotate_cmd == "left") {
    rotateRobot((byte)((rotate_cmd.velocity & MSB_BYTE_BITS) >> BITS_IN_BYTE), (byte)(rotate_cmd.velocity & LSB_BYTE_BITS),
                (byte)(0x00),                                                  (byte)(0x01),
                (byte)((rotate_cmd.angle    & MSB_BYTE_BITS) >> BITS_IN_BYTE), (byte)(rotate_cmd.angle    & LSB_BYTE_BITS));
  }
}

ros::Subscriber<irobot::cmd_rotate> sub_rotate("robot_cmd_rotate", &rotateCmdCb);

ros::Subscriber<irobot::cmd_move>   sub_move  ("robot_cmd_move",   &moveCmdCb);

void stopRobot() {
  //Serial sequence: [137] [Velocity high byte] [Velocity low byte] [Radius high byte] [Radius low byte]
  softSerial.write(137);       // Opcode number for DRIVE
  // Velocity (-500 – 500 mm/s)
  softSerial.write((byte)0);   
  softSerial.write((byte)0);
  // Radius (-2000 – 2000 mm)
  // Special case: straight = 32768 or 32767 = hex 8000 or 7FFF
  softSerial.write((byte)0);  
  softSerial.write((byte)0);   
}

void moveRobot(const byte velocity_msb, const byte velocity_lsb,
               const byte radius_msb,   const byte radius_lsb,
               const byte distance_msb, const byte distance_lsb) {
  //Serial sequence: [137] [Velocity high byte] [Velocity low byte] [Radius high byte] [Radius low byte]
  softSerial.write(137);       // Opcode number for DRIVE
  // Velocity (-500 – 500 mm/s)
  softSerial.write((byte)velocity_msb);  
  softSerial.write((byte)velocity_lsb);
  // Radius (-2000 – 2000 mm)
  // Special case: straight = 32768 or 32767 = hex 8000 or 7FFF
  softSerial.write((byte)radius_msb);  
  softSerial.write((byte)radius_lsb);   

  softSerial.write((byte)156);
  softSerial.write((byte)distance_msb);
  softSerial.write((byte)distance_lsb);

  stopRobot();
}

void rotateRobot(const byte velocity_msb, const byte velocity_lsb,
               const byte radius_msb,     const byte radius_lsb,
               const byte angle_msb,      const byte angle_lsb) {
  //Serial sequence: [137] [Velocity high byte] [Velocity low byte] [Radius high byte] [Radius low byte]
  softSerial.write(137);       // Opcode number for DRIVE
  // Velocity (-500 – 500 mm/s)
  softSerial.write((byte)velocity_msb);  
  softSerial.write((byte)velocity_lsb);
  // Radius (-2000 – 2000 mm)
  // Special case: straight = 32768 or 32767 = hex 8000 or 7FFF
  softSerial.write((byte)radius_msb); 
  softSerial.write((byte)radius_lsb);  

  //delay(2000);
  softSerial.write(157);
  softSerial.write((byte)angle_msb);    
  softSerial.write((byte)angle_lsb);    

  stopRobot();
}

void setup()
{ 
  nh.initNode();
  nh.subscribe(sub_rotate);
  nh.subscribe(sub_move);
  
  delay(2000); // NEEDED!!!! To let the robot initialize 
  
  // define pin modes for software tx, rx pins:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  
  // set the data rate for the SoftwareSerial port, this is the
  // iRobot's default rae of 57600 baud:
  softSerial.begin(57600);

  softSerial.write(128); // This command starts the OI. You must always send the Start command before sending any other commands to the OI
  softSerial.write(131); // safe mode 

}

void loop()
{  
  nh.spinOnce();
  delay(1);
}


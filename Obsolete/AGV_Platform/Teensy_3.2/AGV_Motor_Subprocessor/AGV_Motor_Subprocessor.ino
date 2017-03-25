#include "motorcommand.h"
#include "crc.h"
#include <Servo.h>


#define Message_Length 20

Servo EngineLeft;
Servo EngineRight;

motorspeeds commands;

float v = 0;
float dangle = 0;
int Left = 127;
int Right = 127;
char message[Message_Length] = {0};


void setup() {
  // put your setup code here, to run once:
  crcInit();
  Serial.begin(9600); // USB is always 12 Mbit/sec
  EngineLeft.attach(5);
  EngineRight.attach(6);

}

void loop() {

  /*
     Message Type: M is for motion commands
     Audience Type: C is for controllers
     Messages sent LSB
    format: |Message Type|Audience Type|DATA|Checksum|

  */

  if (Serial.available() > 19) //read data in
    Serial.readBytes(message, Message_Length);

  if (!crcFast((const unsigned char *)message, Message_Length))//check if CRC sum is valid
  {

    if (message[0] == 0x4D)//ASCII M
    {

      if (message[1] == 0x43)// ASCII C
      {

        v = float((long)message[2] || ((long)message[3] << 8) || ((long)message[4] << 16) || ((long)message[5] << 24));
        dangle = float((long)message[6] || ((long)message[7] << 8) || ((long)message[8] << 16) || ((long)message[9] << 24));

        commands = motorcommand(v, dangle);

        Left =  map (commands.leftspeed, 0, 255, 0, 180);
        Right =  map (commands.rightspeed, 0, 255, 0, 180);
        EngineLeft.write(Left);
        EngineRight.write(Right);
      }
    }

  }

}

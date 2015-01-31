

byte inByte = 0;
char array[20];
byte values[18];
int length = 20;
int speedL, speedR = 0;

boolean enable = false;

boolean start, select, L3, R3 = 0;

int LY, LX, RY, RX, Up, Down, Left, Right, X, triangle, square1, circle, L1, L2, R1, R2  = 0;

#include <Servo.h> 
 
Servo EngineLeft;
Servo EngineRight;
 
 
void setup()
{
  Serial.begin(57600);
  EngineLeft.attach(5);
  EngineRight.attach(6);
  
 
}

void loop()
{
  while(Serial.available() > 0)
  {
   
	inByte = Serial.read(); // Read a Byte
        if (inByte == 0)
        {
          
          Serial.readBytes(array, length);
         
          start = array[0]; //start (boolean)
          select = array[1]; //select (boolean)
          values[0] = array[2]; //LY
          values[1] = array[3]; //LX
          values[2] = array[4]; //RY
          values[3] = array[5]; //RX
          values[4] = array[6]; //Up
          values[5] = array[7]; //Down
          values[6] = array[8]; //Left
          values[7] = array[9]; //Right
          values[8] = array[10]; //Triangle
          values[9] = array[11]; //circle
          values[10] = array[12]; // Square
          values[11] = array[13]; //X
          values[12] = array[14]; //L1
          values[13] = array[15]; //L2
          values[14] = array[16]; //L3 (boolean)
          values[15] = array[17]; //R1
          values[16] = array[18]; //R2
          values[17] = array[19]; //R3 (boolean)
          
          
          LY = values[0];
          LX = values[1];
          RY = values[2];
          RX = values[3];
          Up = values[4];
          Down = values[5];
          Left = values[6];
          Right = values[7];
          triangle = values[8];
          circle = values[9];
          square1 = values[10];
          X = values[11];
          L1 = values[12];
          L2 = values[13];
          L3 = values[14];
          R1 = values[15];
          R2 = values[16];
          R3 = values[17];
          
          
          
                      
           speedL =  map (LY, 0, 255, 50, 130);
            speedR =  map (RY, 0, 255, 50, 130);

              EngineLeft.write (speedL);
        
            
             EngineRight.write (speedR);
         }
        
                    
   
  }
}
  



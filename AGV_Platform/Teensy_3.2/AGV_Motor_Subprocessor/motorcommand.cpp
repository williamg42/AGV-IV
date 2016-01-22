#include "motorcommand.h"

motorspeeds motorcommand(float v, float dangle)
{
  motorspeeds commands;
  float velocity;

//Velocity is between 0 and 2 m/s 

if (v == 0)
velocity = 0;

else
velocity = (v) * ((255) / (2)+30);
//left is positive angle, right is negative angle
if(dangle > 0)
{
    commands.leftspeed = velocity*cos(dangle*M_PI/180);
    commands.rightspeed = velocity;    
}

 if(dangle < 0)
 {
    commands.rightspeed = velocity*cos(dangle*M_PI/180);
    commands.leftspeed = velocity;    
 }

 if(dangle == 0)
 {
        
    commands.rightspeed = velocity;
    commands.leftspeed = velocity; 
        
 }

 return commands;
  
}


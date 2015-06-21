#include <PS2X_lib.h>  
#include "crc.h" 


#define PS2_DAT        13     
#define PS2_CMD        11  
#define PS2_SEL        10  
#define PS2_CLK        12  

//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;
unsigned short checksum = 0;
char message[23];
 
void setup(){
 
  Serial.begin(57600);
  crcInit();
  
  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
   
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
	if (pressures)
	  Serial.println("true ");
	else
	  Serial.println("false");
	Serial.print("rumble = ");
	if (rumble)
	  Serial.println("true)");
	else
	  Serial.println("false");
   }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
//  Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
	case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }
}

void loop() {

  if(error == 1) 
   {
    //Safe mode for RC system, sets speed to 0;
   }
  
  else { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

    message[0] = 0;
    message[1] = ps2x.Button(PSB_START);    
    message[2] = ps2x.Button(PSB_SELECT);    
    message[3] = ps2x.Button(PSS_LY);    
    message[4] = ps2x.Button(PSS_LX);    
    message[5] = ps2x.Button(PSS_RY);    
    message[6] = ps2x.Button(PSS_RX);    
    message[7] = ps2x.Button(PSB_PAD_UP);    
    message[8] = ps2x.Button(PSB_PAD_DOWN);    
    message[9] = ps2x.Button(PSB_PAD_LEFT);    
    message[10] = ps2x.Button(PSB_PAD_RIGHT);    
    message[11] = ps2x.Button(PSB_TRIANGLE);
    message[12] = ps2x.Button(PSB_CIRCLE);
    message[13] = ps2x.Button(PSB_SQUARE);
    message[14] = ps2x.Button(PSB_CROSS);
    message[15] = ps2x.Button(PSB_L1);
    message[16] = ps2x.Button(PSB_L2);
    message[17] = ps2x.Button(PSB_L3);
    message[18] = ps2x.Button(PSB_R1);
    message[19] = ps2x.Button(PSB_R2);
    message[20] = ps2x.Button(PSB_R3);



   checksum = crcFast((unsigned char *)message, 21);

   message[21] =   (checksum >> 8);     
   message[22] =   (checksum);

   Serial.write(message, 23);

  }
  delay(50);  
}

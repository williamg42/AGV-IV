#include <PS2X_lib.h>
#include "crc.h"
#include <Wire.h>
#include "U8glib.h"
#include <LiFuelGauge.h>

#define PS2_DAT        13
#define PS2_CMD        11
#define PS2_SEL        10
#define PS2_CLK        12

#define RESET 4
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

#define BATICONX  119
#define BATICONY  0

#define SIGNALX 40
#define SIGNALY 0

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you connect the controller,
//or call config_gamepad(pins) again after connecting the controller.
float SOC = 0;
float VBatt = 0;
int RoundSOC = 0;
int error = 0;
byte type = 0;
byte vibrate = 0;
unsigned short checksum = 0;
char message[23];
int rssiDur;
int rssiValue;

byte EnableTransmission = 0;

char displaymessage[20] = "NO CONNECTION";

char statusmessage[10] = "NONE";

char ReadArr[23];

int displaycycle = 0;

const uint8_t battery_bitmap[] U8G_PROGMEM = {
  0b00011000,
  0b01111110,
  0b01000010,
  0b01000010,
  0b01000010,
  0b01000010,
  0b01000010,
  0b01000010,
  0b01000010,
  0b01000010,
  0b01000010,
  0b01000010,
  0b01111110  
};

const uint8_t battery_Charging_bitmap[] U8G_PROGMEM = {
  0b00011000,
  0b01111110,
  0b01111010,
  0b01110110,
  0b01101110,
  0b01011110,
  0b01101110,
  0b01110110,
  0b01111010,
  0b01110110,
  0b01101110,
  0b01011110,
  0b01111110  
};

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send ACK

LiFuelGauge gauge(MAX17043, 0, lowPower);
volatile boolean alert = false;

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here

  u8g.drawHLine(0, 19, 128);

  u8g.setFont(u8g_font_u8glib_4);
if (error == 0) {
  u8g.setPrintPos(0, 17);
 u8g.print("Controller Detected");
}
else {
  u8g.setPrintPos(0, 17);
 u8g.print("Controller Error");
}

 u8g.setPrintPos(90, 17);


  if (pressures)
     u8g.print("Analog");
    else
    u8g.print("Digital");

 if (EnableTransmission == 1)
    {
       u8g.setFont(u8g_font_u8glib_4);
       u8g.setPrintPos(0, 49);
      u8g.print("Transmission Enabled");
    }
    else
    {
            u8g.setFont(u8g_font_u8glib_4);
       u8g.setPrintPos(0, 49);
      u8g.print("Transmission Disabled"); 
    }


u8g.setFont(u8g_font_5x7);
  u8g.setPrintPos(0, 28);
 u8g.print("Messages:");
  u8g.setPrintPos(0, 38);
 u8g.print(displaymessage);
 
 u8g.drawHLine(0, 54, 128);

u8g.setPrintPos(0, 62);
 u8g.print("System Status:");

 u8g.setPrintPos(72, 62);
 u8g.print(statusmessage);
   
if (RoundSOC >= 100)
{
u8g.setFont(u8g_font_5x7);
u8g.setPrintPos(BATICONX-22, BATICONY+10);
  u8g.print(RoundSOC);
  u8g.setPrintPos(BATICONX-6, BATICONY+10);
  u8g.print("%");
}

else
{
  u8g.setFont(u8g_font_5x7);
u8g.setPrintPos(BATICONX-17, BATICONY+10);
  u8g.print(RoundSOC);
  u8g.setPrintPos(BATICONX-6, BATICONY+10);
  u8g.print("%");  
}

  if (VBatt > 4.19)
  {
u8g.drawBitmapP(BATICONX,BATICONY,1,13,battery_Charging_bitmap); 
  }
  else
  {
u8g.drawBitmapP(BATICONX,BATICONY,1,13,battery_bitmap);

if (RoundSOC >= 95)
{
  u8g.drawBox(BATICONX+2,BATICONY+2,4,10); 
}
else if (RoundSOC >= 85 && RoundSOC < 95)
{
  u8g.drawBox(BATICONX+2,BATICONY+3,4,9); 
}

else if (RoundSOC >= 75 && RoundSOC < 85)
{
  u8g.drawBox(BATICONX+2,BATICONY+4,4,8); 
}

else if (RoundSOC >= 65  && RoundSOC < 75)
{
  u8g.drawBox(BATICONX+2,BATICONY+5,4,7); 
}
else if (RoundSOC >= 55  && RoundSOC < 65)
{
  u8g.drawBox(BATICONX+2,BATICONY+6,4,6); 
}

else if (RoundSOC >= 45  && RoundSOC < 55)
{
  u8g.drawBox(BATICONX+2,BATICONY+7,4,5); 
}

else if (RoundSOC >= 35  && RoundSOC < 45)
{
  u8g.drawBox(BATICONX+2,BATICONY+8,4,4); 
}
     
  }


 u8g.setFont(u8g_font_5x7);
    u8g.setPrintPos(SIGNALX-40, SIGNALY+10);
   u8g.print("RSSI:");
switch(rssiValue)
{
  
  case 0:
  {
    u8g.setFont(u8g_font_5x7);
    u8g.setPrintPos(SIGNALX-10, SIGNALY+10);
   u8g.print("X");

 break;
  }

  
  case 1:
  {
  u8g.drawBox(SIGNALX-15,SIGNALY+8,4,2);//small

    break;

  }

  case 2:
  {
   u8g.drawBox(SIGNALX-10,SIGNALY+6,4,4);

   u8g.drawBox(SIGNALX-15,SIGNALY+8,4,2);//small


    break;

    
  }

  case 3:
  {

   u8g.drawBox(SIGNALX-5,SIGNALY+4,4,6);

   u8g.drawBox(SIGNALX-10,SIGNALY+6,4,4);

   u8g.drawBox(SIGNALX-15,SIGNALY+8,4,2);//small


   break;  
    
  }

  case 4:
  {
     u8g.drawBox(SIGNALX,SIGNALY+2,4,8);  

   u8g.drawBox(SIGNALX-5,SIGNALY+4,4,6);

   u8g.drawBox(SIGNALX-10,SIGNALY+6,4,4);

   u8g.drawBox(SIGNALX-15,SIGNALY+8,4,2);//small

   break;
    
  }
}
}



void setup() {


   if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }

  
 

  Serial.begin(57600);
  
  //crcInit();
  gauge.reset(); // Resets MAX17043
  delay(200); // Waits for the initial measurements to be made
// Sets the Alert Threshold to 10% of full capacity
  gauge.setAlertThreshold(10);

  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
 
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
 error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

}




void loop() {

  SOC = gauge.getSOC();
    VBatt = gauge.getVoltage();
RoundSOC = SOC;

  // picture loop
  if (displaycycle == 10)
  {    
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  displaycycle = 0;
  }

  

 rssiDur = pulseIn(6, LOW, 200);
 rssiValue = map(rssiDur, 0, 200, 0, 4);

if(Serial.available() >= 23)
{
  Serial.readBytes(ReadArr,23);
  if(crcSlow((unsigned char *)ReadArr, 23) > 0)
  {
    //error, bad message
  }

  else
  {
    //good message, parse data
    switch(ReadArr[0])
    {
      case 17:
      {
         memcpy(displaymessage, &ReadArr[1], sizeof(byte) * 20);
        break;
      }

      case 34:
      {

         memcpy(statusmessage, &ReadArr[1], sizeof(byte) * 10);

        break;
      }

      default:
      {
        
        break;
      }
    }
    
  }
}
 
  if (error == 1)
  {
    //Controller not detected. Sending all off data
    
        message[0] = 0;
    message[1] = 0; //ps2x.Button(PSB_START);
    message[2] = 0;//ps2x.Button(PSB_SELECT);
    message[3] = 127; //ps2x.Button(PSS_LY);
    message[4] = 127; //ps2x.Button(PSS_LX);
    message[5] = 127; //ps2x.Button(PSS_RY);
    message[6] = 127; //ps2x.Button(PSS_RX);
    message[7] = 0; //ps2x.Button(PSB_PAD_UP);
    message[8] = 0; //ps2x.Button(PSB_PAD_DOWN);
    message[9] = 0; //ps2x.Button(PSB_PAD_LEFT);
    message[10] = 0; //ps2x.Button(PSB_PAD_RIGHT);
    message[11] = 0; //ps2x.Button(PSB_TRIANGLE);
    message[12] = 0; //ps2x.Button(PSB_CIRCLE);
    message[13] = 0; //ps2x.Button(PSB_SQUARE);
    message[14] = 0; //ps2x.Button(PSB_CROSS);
    message[15] = 0; //ps2x.Button(PSB_L1);
    message[16] = 0; // ps2x.Button(PSB_L2);
    message[17] = 0; //ps2x.Button(PSB_L3);
    message[18] = 0; //ps2x.Button(PSB_R1);
    message[19] = 0; //ps2x.Button(PSB_R2);
    message[20] = 0; //ps2x.Button(PSB_R3);
    checksum = crcSlow((unsigned char *)message, 21);
    message[21] =   (checksum >> 8);
    message[22] =   (checksum);
    Serial.write(message, 23);
    
    
  }

  else { //DualShock Controller
    ps2x.read_gamepad(); //read controller

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
    
  }
    
    if ( message[1]  == 1)
    {
      EnableTransmission = !EnableTransmission;
    }
    else
    {
      EnableTransmission = EnableTransmission;
    }

//memset(&arr[0], 0, sizeof(arr));

    checksum = crcSlow((unsigned char *)message, 21);

    message[21] =   (checksum >> 8);
    message[22] =   (checksum);

  if (EnableTransmission == 1)
    {
      Serial.write(message, 23);
    }
    else
    {      
    message[0] = 0;
    message[1] = 0; //ps2x.Button(PSB_START);
    message[2] = 0;//ps2x.Button(PSB_SELECT);
    message[3] = 127; //ps2x.Button(PSS_LY);
    message[4] = 127; //ps2x.Button(PSS_LX);
    message[5] = 127; //ps2x.Button(PSS_RY);
    message[6] = 127; //ps2x.Button(PSS_RX);
    message[7] = 0; //ps2x.Button(PSB_PAD_UP);
    message[8] = 0; //ps2x.Button(PSB_PAD_DOWN);
    message[9] = 0; //ps2x.Button(PSB_PAD_LEFT);
    message[10] = 0; //ps2x.Button(PSB_PAD_RIGHT);
    message[11] = 0; //ps2x.Button(PSB_TRIANGLE);
    message[12] = 0; //ps2x.Button(PSB_CIRCLE);
    message[13] = 0; //ps2x.Button(PSB_SQUARE);
    message[14] = 0; //ps2x.Button(PSB_CROSS);
    message[15] = 0; //ps2x.Button(PSB_L1);
    message[16] = 0; // ps2x.Button(PSB_L2);
    message[17] = 0; //ps2x.Button(PSB_L3);
    message[18] = 0; //ps2x.Button(PSB_R1);
    message[19] = 0; //ps2x.Button(PSB_R2);
    message[20] = 0; //ps2x.Button(PSB_R3);
    checksum = crcSlow((unsigned char *)message, 21);
    message[21] =   (checksum >> 8);
    message[22] =   (checksum);
    Serial.write(message, 23);
      
    }
  
  delay(10);

  displaycycle = displaycycle +1;
}

void lowPower() { alert = true; }

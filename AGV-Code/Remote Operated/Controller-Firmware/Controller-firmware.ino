#include <PS2X_lib.h>
#include "crc.h"
#include <Wire.h>
#include "U8glib.h"
#include <LiFuelGauge.h>

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
String BatteryCharge;
String Voltage;
int error = 0;
byte type = 0;
byte vibrate = 0;
unsigned short checksum = 0;
char message[23];
int rssiDur;

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send ACK

LiFuelGauge gauge(MAX17043, 0, lowPower);
volatile boolean alert = false;

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21); gauge.getVoltage()
  u8g.setPrintPos(5, 44);
   u8g.print("SOC:");
   u8g.setPrintPos(0, 64);
  u8g.print(gauge.getSOC());

    u8g.setPrintPos(95, 44);
   u8g.print("V:");
   u8g.setPrintPos(80, 64);
  u8g.print(gauge.getVoltage());

    
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

  delay(1000);  //added delay to give wireless ps2 module some time to startup, before configuring it
 
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

  if (error == 0) {
    Serial.println("Found Controller, configured successful ");
    Serial.println("pressures = ");
    if (pressures)
      Serial.println("true ");
    else
    Serial.println("false");
    
    Serial.println("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");
  }
  else if (error == 1)
  {
    
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
  }
  else if (error == 2)
  {
    
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
  }
  else if (error == 3)
  {
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  }

}




void loop() {

  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );

  

 rssiDur = pulseIn(6, LOW, 200);
  //Serial.println("Voltage = : SOC = : ");
 
  if (error == 1)
  {
    //Safe mode for RC system, sets speed to 0;
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



    checksum = crcSlow((unsigned char *)message, 21);

    message[21] =   (checksum >> 8);
    message[22] =   (checksum);

    Serial.write(message, 23);

  }
  delay(50);
}

void lowPower() { alert = true; }

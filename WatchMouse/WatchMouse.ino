#define BLU_DEBUG true
#include <SPI.h>
#include <Wire.h>
#include <TinyScreen.h>
#include <STBLE.h>
#include "USBHID_Types.h"
#include "UUID.h"
#include "BMA250.h" 

// Accelerometer sensor variables for the sensor and its values
BMA250 accel_sensor;



#if defined(ARDUINO_ARCH_SAMD)
 #define SerialMonitorInterface SerialUSB
#else
 #define SerialMonitorInterface Serial
#endif

#define SerialMonitorInterface SerialUSB

TinyScreen display = TinyScreen(TinyScreenDefault);


/*
Bluetooth stuff.
*/
BLEConnection bleconn;
uint8_t ble_connection_state = false;

uint8_t curr_mouse_report[] = {0,0,0,0};
uint8_t old_mouse_report[] = {0,0,0,0};

uint8_t curr_media_report[] = {0,0,0,0,0,0,0,0};
uint8_t old_media_report[] = {0,0,0,0,0,0,0,0};


long BLE_Timer = 0;

int menu_page_number = 1;

//set to zero if you don't want to listen to mouse controls
//uint8_t mouse_mode = 1;


void setup() {

  Wire.begin();
  SerialMonitorInterface.begin(9600); //baud rate or something
  //while(!SerialMonitorInterface); //make sure we can print stuff in SMI, before we proceed

  display.begin();
  BLE_Setup();
  // Set up the BMA250 acccelerometer sensor
  accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms); 
}

void loop() {
  
  BLE_Loop();

  //Accelerometer();
}

/*
Bluetooth stuff
*/
void HID_onConnect(){
  SerialMonitorInterface.println("onConnect triggered");
  requestSecurity();
  //doBond()
}

void HID_onDisconnect(){
  //doDisconnect()
  //ble_connection_state = false;
  setDiscoverable();
}
void HID_onBond(){
  SerialMonitorInterface.println("bond event");
  ble_connection_state = true;
}
/*
End of bluetooth stuff
*/
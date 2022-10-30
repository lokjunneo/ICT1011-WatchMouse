#define BLU_DEBUG true
#include <SPI.h>
#include <Wire.h>
#include <TinyScreen.h>
#include <STBLE.h>
#include "USBHID_Types.h"
#include "UUID.h"

#define SerialMonitorInterface SerialUSB

TinyScreen display = TinyScreen(TinyScreenDefault);

/*
Bluetooth stuff.
Set mouse_mode to 0 to disable mouse control
Set mouse_mode to 1 to enable mouse control
Must remember to turn it back on somehow if you on it
If you are not sure, just turn it off
*/
BLEConnection bleconn;
uint8_t ble_connection_state = false;

uint8_t curr_mouse_report[] = {0,0,0,0};

long BLE_Timer = 0;

//set to zero if you don't want to listen to mouse controls
//uint8_t mouse_mode = 1;


void setup() {

  Wire.begin();
  SerialMonitorInterface.begin(9600); //baud rate or something
  //while(!SerialMonitorInterface); //make sure we can print stuff in SMI, before we proceed

  display.begin();
  BLE_Setup();

}

void loop() {
  
  BLE_Loop();

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
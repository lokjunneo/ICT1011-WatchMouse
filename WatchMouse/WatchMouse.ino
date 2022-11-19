#define BLU_DEBUG true
#include <SPI.h>
#include <Wire.h>
#include <TinyScreen.h>
#include <STBLE.h>

#include <TimeLib.h> //include the Arduino Time library
#include <RTCZero.h>
#include <Time.h>

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
Menu stuff
*/

int menu_page_number = 1;
/*
Bluetooth stuff.
*/
BLEConnection bleconn;
uint8_t ble_connection_state = false;

int8_t ble_drawn_menu_page = 0;
int8_t ble_drawn_remote_page = 0;

uint8_t curr_mouse_report[] = {0,0,0,0};
uint8_t old_mouse_report[] = {0,0,0,0};

uint8_t curr_media_report[] = {0,0,0,0,0,0,0,0};
uint8_t old_media_report[] = {0,0,0,0,0,0,0,0};

uint8_t curr_keyboard_report[] = {0,0};
uint8_t old_keyboard_report[] = {0,0};
int toggleBtn=1;
long stopwatch_time_when_last_button_was_pressed = 0;






void setup() {

  Wire.begin();
  SerialMonitorInterface.begin(9600); //baud rate or something
  //while(!SerialMonitorInterface); //make sure we can print stuff in SMI, before we proceed
  display.begin();
  display.setFlip(1);
  BLE_Setup();
  displayclock_setup();
  // Set up the BMA250 acccelerometer sensor
  accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms); 
  toggleBtn=1;
  stopwatch_time_when_last_button_was_pressed = 0;
}

void loop() {
  
  BLE_Loop();
  stopwatch_btmleftBtn();
  stopwatch_toprightBtn();
  stopwatch_topleftBtn();
  stopwatch_toprightBtn_2();
  stopwatch_draw_loop();
  clock_update();

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
  ble_drawn_menu_page = 0;
}
void HID_onBond(){
  SerialMonitorInterface.println("bond event");
  ble_connection_state = true;
  ble_drawn_menu_page = 0;
}
/*
End of bluetooth stuff
*/
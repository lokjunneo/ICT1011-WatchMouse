/*
Use this functions I prepared:
1) updateMouseXY(x,y): change mouse position by x and y.  x and y are signed integer 8 bit (int8_t)
2) Leave BLE_Setup() in setup()
3) Leave BLE_Loop() in loop()
4) use ble_connection_state to determine that bluetooth is connected
ble_connection_state = true means it is connected
*/

//Call this in setup()

void BLE_Setup(){

  BLE_Init("WatchMouse");
  BLE_Conn_Setup(&bleconn, HID_onConnect, HID_onDisconnect);
  init_HID_service();
  init_Security(HID_onBond);
  setDiscoverable();

}


void BLE_Loop(){
  BLE_Process();
  if (ble_connection_state){
    bleCheckButton();
    if (millis() - BLE_Timer > 30){
      update_mouse(curr_mouse_report);
      BLE_Timer = millis();
      reset_mouse_report();
    }
  }
}
//To do: add left and right click

void bleCheckButton(){
  if (display.getButtons(TSButtonUpperLeft)){
    updateMouseXY(0,1);
  }
  else if (display.getButtons(TSButtonUpperRight)){
    updateMouseXY(0,-1);
  }
}

//use this to set how much 
uint8_t updateMouseXY(int8_t offsetX,int8_t offsetY){
  int8_t currX = (int8_t)curr_mouse_report[1];
  int8_t newX = offsetX + currX;
  int8_t currY = (int8_t)curr_mouse_report[2];
  int8_t newY = offsetY+ currY;
  //overflow: -ve + -ve give positive
  if (offsetX < 0 && currX < 0 && newX > 0){ newX = -128;}
  //overflow: +ve + +ve give negative
  else if (offsetX > 0 && currX > 0 && newX < 0) {newX = 127;}

  if (offsetY < 0 && currY < 0 && newY > 0){ newY = -128;}
  else if (offsetY > 0 && currY > 0 && newY < 0) {newY = 127;}

  curr_mouse_report[1] = newX;
  curr_mouse_report[2] = newY;
}

uint8_t reset_mouse_report(){
  curr_mouse_report[0] = 0;
  curr_mouse_report[1] = 0;
  curr_mouse_report[2] = 0;
  curr_mouse_report[3] = 0;
 }

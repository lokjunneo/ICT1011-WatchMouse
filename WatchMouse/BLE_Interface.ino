/*
Use this functions I prepared:
1) updateMouseXY(x,y): change mouse position by x and y.  x and y are signed integer 8 bit (int8_t)
2) Leave BLE_Setup() in setup()
3) Leave BLE_Loop() in loop()
4) use ble_connection_state to determine that bluetooth is connected
ble_connection_state = true means it is connected
*/

//Call this in setup()

long ble_millis = 0; //to store millis(), so we only call it once a loop

const uint8_t ble_total_page = 2;
uint8_t ble_curr_page_number = 0;
uint8_t ble_update_delay = 30;

//Tried to have a function to store the pointer to functions, but it affected other unrelated global array value
uint8_t (*ble_updater[ble_total_page])() = {ble_mouse_updater, ble_media_updater};
uint8_t (*ble_controller[ble_total_page])() = {ble_mouse_ud_move, ble_media_vol};

void BLE_Setup(){

  BLE_Init("WatchMouse");
  BLE_Conn_Setup(&bleconn, HID_onConnect, HID_onDisconnect);
  init_HID_service();
  init_Security(HID_onBond);
  setDiscoverable();

}


void BLE_Loop(){
  BLE_Process();
  ble_millis = millis();
  if (ble_connection_state){
    bleCheckButton();
    if (ble_millis - BLE_Timer > ble_update_delay){
      bleUpdater();
    }
    else if (ble_millis - BLE_Timer < 0) {BLE_Timer = 0;} //millis overflows after roughly 50 days
  }
}
//To do: add left and right click


void bleCheckButton(){
  //ble_mouse_ud_move();
  /*
  switch (ble_curr_page_number){
    case 0:
      ble_mouse_ud_move();
      break;
  }*/
  if ( *ble_controller[ble_curr_page_number]) {(*ble_controller[ble_curr_page_number])();}
}

void bleUpdater(){
  /*
  switch (ble_curr_page_number){
    case 0:
      ble_mouse_updater();
      break;
    
  }*/
  if ( *ble_updater[ble_curr_page_number] ) {(*ble_updater[ble_curr_page_number])();}
}

/*
  Mouse related functions
*/

//Detect button presses
uint8_t ble_mouse_ud_move(){
  if (display.getButtons(TSButtonUpperLeft)){
    updateMouseXY(50,50);
  }
  else if (display.getButtons(TSButtonUpperRight)){
    updateMouseXY(100,100);
  }

}
/*
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

  //ble_call_update = 40;
}*/

uint8_t updateMouseXY(int8_t newX,int8_t newY){
  curr_mouse_report[1] = newX;
  curr_mouse_report[2] = newY;

  //ble_call_update = 40;
}

uint8_t ble_mouse_updater(){
  if (mouse_report_changed()){
      update_mouse(curr_mouse_report);
      BLE_Timer = millis();
      reset_mouse_report();
  }
}

uint8_t reset_mouse_report(){
  old_mouse_report[0] = curr_mouse_report[0];
  old_mouse_report[1] = curr_mouse_report[1];
  old_mouse_report[2] = curr_mouse_report[2];
  old_mouse_report[3] = curr_mouse_report[3];
  curr_mouse_report[0] = 0;
  curr_mouse_report[1] = 0;
  curr_mouse_report[2] = 0;
  curr_mouse_report[3] = 0;
}

uint8_t mouse_report_changed(){
  uint8_t curr_mouse_total = 0;
  uint8_t old_mouse_total = 0;
  uint8_t ret = 0;
  for (int i = 0; i < 4; i++){  
    curr_mouse_total += curr_mouse_report[i];
    old_mouse_total += old_mouse_report[i];
    if (curr_mouse_total){
      ret = 1;
      break;
    }
  }
  return (curr_mouse_total || old_mouse_total);
} 

/*
  Media Related functions
*/

//handles updating of ble characteristic: media
uint8_t ble_media_updater(){
  if (media_report_changed()){
    update_media(curr_media_report);
    BLE_Timer = millis();
    reset_media_report();
  }
}

//detect button presses
uint8_t ble_media_vol(){
  if (display.getButtons(TSButtonUpperLeft)){
    curr_media_report[5] = 5;
  }
  else if (display.getButtons(TSButtonUpperRight)){
    curr_media_report[6] = 6;
  }
}

uint8_t media_report_changed(){
  uint8_t curr_media_total = 0;
  uint8_t old_media_total = 0;
  uint8_t ret = 0;
  for (int i = 0; i < 8; i++){  
    curr_media_total += curr_media_report[i];
    old_media_total += old_media_report[i];
    if (curr_media_total){
      ret = 1;
      break;
    }
  }
  if (curr_media_total){
  SerialMonitorInterface.println(curr_media_total);}
  return (curr_media_total || old_media_total);
} 

uint8_t reset_media_report(){
  for (int i = 0; i<sizeof(curr_media_report); i++){
    old_media_report[i] = curr_media_report[i];
    curr_media_report[i] = 0;
  }
}



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

long BLE_Timer = 0; //used to delay updating of bluetooth characteristics
long ble_command_timer = 0; //delay for command buttons (Lower Left and Right), so user wont accidentally scroll through multiple pages at once


//Tried to have a function to store the pointer to functions, but it affected other unrelated global array value
uint8_t (*ble_updater[ble_total_page])() = {ble_media_updater, ble_mouse_updater};
uint8_t (*ble_controller[ble_total_page])() = {ble_media_vol, ble_mouse_accel_move};
char * ble_page_text[ble_total_page] = {"Volume","Mouse"}; 
uint8_t ble_page_text_size[ble_total_page] = {6,5};

void BLE_Setup(){

  BLE_Init("WatchMouse");
  BLE_Conn_Setup(&bleconn, HID_onConnect, HID_onDisconnect);
  init_HID_service();
  init_Security(HID_onBond);
  setDiscoverable();

}


void BLE_Loop(){
  BLE_Process();
  ble_menu_page_check();
  ble_rightBtn();
  ble_millis = millis();
  if (ble_millis < BLE_Timer){
    BLE_Timer = 0;
    ble_command_timer = 0;
  }
  if (ble_connection_state && ble_curr_page_number){
    ble_remote_page_check();
    bleCheckButton();
    if (ble_millis - BLE_Timer > ble_update_delay){
      bleUpdater();
    }
    else if (ble_millis - BLE_Timer < 0) {BLE_Timer = 0;} //millis overflows after roughly 50 days
    ble_leftBtn();
  }
}
//To do: add left and right click


void bleCheckButton(){
  if ( *ble_controller[ble_curr_page_number-1]) {(*ble_controller[ble_curr_page_number-1])();}
}

void bleUpdater(){
  if ( *ble_updater[ble_curr_page_number-1] ) {(*ble_updater[ble_curr_page_number-1])();}
}

/*
  Mouse related functions
*/

//Detect button presses
uint8_t ble_mouse_accel_move(){
  
  if (display.getButtons(TSButtonUpperLeft)){
    updateMouseLR_Click(1,0);
  }
  else if (display.getButtons(TSButtonUpperRight)){
    updateMouseLR_Click(0,1);
  }
  accelerometer_mouse();

}

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
}

uint8_t updateMouseLR_Click(bool left, bool right){
  curr_mouse_report[0] |= left;
  curr_mouse_report[0] |= right*2;
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
    curr_media_report[0] = 64;
    curr_media_report[1] = 0;
  }
  else if (display.getButtons(TSButtonUpperRight)){
    curr_media_report[0] = 32;
    curr_media_report[1] = 0;
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

void ble_quick_draw(int draw_x, int draw_y, char text[], const int text_size){
  display.setFont(thinPixel7_10ptFontInfo);
  char str[text_size+1];
  str[text_size] = '\0'; //terminating character
  for (int i = 0; i<text_size; i++){
    str[i] = text[i];
  } 
  display.setCursor(draw_x, draw_y);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.print(str);
}

/*
  Check if currently on page 1 of the menu, and check if buttons have been drawn
*/
uint8_t ble_rightBtn(){
  if (display.getButtons(TSButtonLowerRight)){
    if (ble_millis - ble_command_timer > 200){
      if (menu_page_number <= 1){
        if (menu_page_number){
          if (ble_connection_state){
            menu_page_number = 0;
            ble_drawn_menu_page = 0;
            ble_curr_page_number = 1;
          }
        }
        else {
          if ((ble_curr_page_number +1) <= ble_total_page){
            ble_curr_page_number++;
            SerialMonitorInterface.println("Incrementing");
            SerialMonitorInterface.println(ble_drawn_menu_page);
          }
        }
        ble_command_timer = millis();  
      }

      SerialMonitorInterface.println("Right btn clicked");
      ble_debug_print();
    }
  }
}

uint8_t ble_leftBtn(){
  if (display.getButtons(TSButtonLowerLeft)){
    if (ble_millis - ble_command_timer > 200){
      if (menu_page_number < 1){
        if (ble_curr_page_number -1 > 0){
          ble_curr_page_number--;
        }
        else {
          ble_curr_page_number = 0;
          menu_page_number = 1;
          ble_drawn_remote_page = 0;
          display.clearScreen();
          
        }
        ble_command_timer = millis();
      }  
      SerialMonitorInterface.println("Left btn clicked");
      ble_debug_print();
    }
  }
}


uint8_t ble_menu_page_check(){
  if (menu_page_number == 1){
    if(!ble_drawn_menu_page){
      if (ble_connection_state){
        ble_quick_draw(51,50,"Remote >", 8);
      }
      else{
        ble_quick_draw(51,50,"Remote x", 8);
      }
    ble_drawn_menu_page = 1;
    }
  }
  else {
    ble_drawn_menu_page = 0;
  }
}

uint8_t ble_remote_page_check(){
  if (menu_page_number == 0){
    if (ble_drawn_remote_page != ble_curr_page_number){
      display.clearScreen();
      ble_remote_page_bar_print();
      //if (ble_drawn_remote_page){
      switch (ble_curr_page_number){
        case 1:
          ble_quick_draw(90,0, "+", 1);
          ble_quick_draw(0,0,"-", 1);
          break;
        case 2:
          ble_quick_draw(90,0,"R",1);
          ble_quick_draw(0,0,"L",1);
          break;
      }
      ble_drawn_remote_page = ble_curr_page_number;     
      //}   
    }
  }
}

void ble_debug_print(){
  SerialMonitorInterface.print("Menu page number is now: ");
  SerialMonitorInterface.print(menu_page_number);
  SerialMonitorInterface.print(", ble_curr_page_number is now: ");
  SerialMonitorInterface.print(ble_curr_page_number);
  SerialMonitorInterface.print(", ble_drawn_menu_page is now: ");
  SerialMonitorInterface.print(ble_drawn_menu_page);
  SerialMonitorInterface.print(", ble_drawn_remote_page is now: ");
  SerialMonitorInterface.println(ble_drawn_remote_page);
}

/*
  Print out the navigation bar, as stored in ble_page_text
*/
uint8_t ble_remote_page_bar_print(){
  //If there is still one remote control page before
  if (ble_curr_page_number-2 >= 0){
    ble_quick_draw(0,50,"<", 1);
    ble_quick_draw(5,50, ble_page_text[ble_curr_page_number-2], ble_page_text_size[ble_curr_page_number-2]);
  }
  //If there is no more remmote control page before this (Would be menu page before this)
  else {
    ble_quick_draw(0,50,"< Back", 6);
  }
  if (ble_curr_page_number +1 <= ble_total_page){
    int strLen = ble_page_text_size[ble_curr_page_number];
    ble_quick_draw(90-(min(90,strLen*6)),50, ble_page_text[ble_curr_page_number], strLen);
    ble_quick_draw(90,50,">", 1);
  }
  int strLen = ble_page_text_size[ble_curr_page_number-1];
  ble_quick_draw(50 - min(50,(strLen * 3)),0, ble_page_text[ble_curr_page_number-1], strLen);
}
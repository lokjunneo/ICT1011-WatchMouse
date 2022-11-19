//#include <TinyScreen.h>

//#include <SPI.h>
unsigned long stopwatch_startMillis;  //some global variables available anywhere in the program
unsigned long stopwatch_currentMillis;
unsigned long stopwatch_oldStorageMillis=0;
unsigned long stopwatch_storageSeconds;
unsigned long stopwatch_storageMillis;
unsigned long stopwatch_storageMinute;
unsigned long stopwatch_pauseMillis;
unsigned long stopwatch_pauseSeconds;
unsigned long stopwatch_pauseMinutes;
const unsigned long stopwatch_period = 100;





void stopwatch_setup() {
  // put your setup code here, to run once:
  toggleBtn=1;
  Serial.begin(9600);
  display.on();
  display.begin();
  display.setFont(thinPixel7_10ptFontInfo);
  stopwatch_startMillis = millis();  //initial start time
  
  
}

void stopwatch_draw_loop() {

  if (menu_page_number == 10){
    
    
    
      

    stopwatch_currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
    if(toggleBtn==1){
    stopwatch_storageMillis = (stopwatch_currentMillis-stopwatch_startMillis);
    stopwatch_storageMillis += stopwatch_oldStorageMillis;
    stopwatch_storageSeconds = (stopwatch_storageMillis/1000);
    //stopwatch_storageSeconds =(abs(stopwatch_storageSeconds%1000));
    stopwatch_storageMinute =(stopwatch_storageSeconds/60);
    //stopwatch_storageMinute =(abs(stopwatch_storageMinute%60));
    display.setFont(thinPixel7_10ptFontInfo);
    display.setCursor(15,8);
    
    if (stopwatch_currentMillis - stopwatch_startMillis >= stopwatch_period)  //stopwatch whether the period has elapsed
    {
      
      
      //print out the time
      display.setCursor(0,0);
      display.print("P/S");
      display.setCursor(0,55);
      display.print("<");
      display.setCursor(90,0);
      display.print("R");
      display.setCursor(15,32);
      display.print(abs(stopwatch_storageMinute%60));
      display.print(" : ");
      display.print(abs(stopwatch_storageSeconds%60));
      display.print(" : ");
      display.print(stopwatch_storageMillis%1000);
      display.print(" ");
      display.setCursor(15,52);
      display.print(toggleBtn);
      stopwatch_pauseMillis =millis()-stopwatch_storageMillis;
      stopwatch_pauseSeconds =abs(stopwatch_storageSeconds%60);
      stopwatch_pauseMinutes=abs(stopwatch_storageMinute%60);
      
      

      

      //stopwatch_startMillis = stopwatch_currentMillis;  //IMPORTANT to save the start time of the current LED state.
    }
  }
  else if (toggleBtn==0){
    stopwatch_startMillis = stopwatch_currentMillis;  //IMPORTANT to save the start time of the current LED state.
    
      display.setCursor(0,0);
      display.print("P/S");
      display.setCursor(0,55);
      display.print("<");
      display.setCursor(90,0);
      display.print("R");
      display.setCursor(15,32);
      display.print(stopwatch_pauseMinutes);
      display.print(" : ");
      display.print(stopwatch_pauseSeconds);
      display.print(" : ");
      display.print(stopwatch_pauseMillis%1000);
      display.print(" ");
      display.setCursor(15,52);
      display.print(toggleBtn);
      

      //stopwatch_storageMillis=stopwatch_pauseMillis;
      //stopwatch_storageSeconds=abs(stopwatch_pauseSeconds%60);
      //stopwatch_storageMinute=abs(stopwatch_pauseMinutes%60);
      //SerialMonitorInterface.print("\n");

      //SerialMonitorInterface.print(stopwatch_pauseMillis);
     // SerialMonitorInterface.print("\n");
      //SerialMonitorInterface.print(stopwatch_pauseSeconds);
      //SerialMonitorInterface.print("\n");
      //SerialMonitorInterface.print(stopwatch_pauseMinutes);


  }
  }
}



uint8_t stopwatch_toprightBtn(){
  if (display.getButtons(TSButtonUpperRight)){
      if (menu_page_number <= 1){
        if (menu_page_number){
          menu_page_number = 10;
          stopwatch_startMillis = millis();
          display.clearScreen();
        }
        

      SerialMonitorInterface.println("Right btn clicked");
      ble_debug_print();
    }
  }
}
uint8_t stopwatch_btmleftBtn(){
  if(display.getButtons(TSButtonLowerLeft)){
    if(menu_page_number ==10){
      display.clearScreen();
      menu_page_number=1;
      //todo:
    }
  }
}

uint8_t stopwatch_topleftBtn(){
  if(display.getButtons(TSButtonUpperLeft)){
    if(menu_page_number ==10 && millis() - stopwatch_time_when_last_button_was_pressed > 350){
      stopwatch_time_when_last_button_was_pressed = millis();

      if(toggleBtn ==0){
        
        stopwatch_startMillis = millis();
        toggleBtn =1;
        
      }else{
        stopwatch_oldStorageMillis  = stopwatch_storageMillis;
        toggleBtn=0;
        
      }
      //todo:
    }
  }
}
uint8_t stopwatch_toprightBtn_2(){
  if(display.getButtons(TSButtonUpperRight)){
    if(menu_page_number ==10 && millis() - stopwatch_time_when_last_button_was_pressed > 350){
      stopwatch_time_when_last_button_was_pressed = millis();
      if(toggleBtn==0){
      display.clearScreen();
      stopwatch_storageMillis = 0;
      stopwatch_storageSeconds=0;
      stopwatch_storageMinute=0;
      stopwatch_oldStorageMillis=0;
      stopwatch_pauseMinutes=0;
      stopwatch_pauseSeconds=0;
      stopwatch_pauseMillis=0;


      }
      
    }
  }
}
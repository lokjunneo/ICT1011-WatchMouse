/*
 * Simple Watch
 *
 * Demonstrates the use of the Arduino Time library to make a simple digital watch
 *
 * Uses Arduino Time library http://playground.arduino.cc/code/time  
 * Maintained by Paul Stoffregen https://github.com/PaulStoffregen/Time
 *
 * This code is for the TinyScreen+ board by TinyCircuits used by O Watch http://tiny-circuits.com 
 *
 * This example is created by O Watch on 6 March 2016 http://theowatch.com 
 * 
*/
long clock_millis=0;
int set_time_track = 0;



#include <TinyScreen.h> //include TinyScreen library
#include <TimeLib.h> //include the Arduino Time library
#include <SPI.h> //include this library for communication with OLED screen
#include <Wire.h>

//TinyScreen display = TinyScreen(TinyScreenDefault); //Create the TinyScreen object
#if defined(ARDUINO_ARCH_SAMD)
 #define SerialMonitorInterface SerialUSB
#else
 #define SerialMonitorInterface Serial
#endif

#define SerialMonitorInterface SerialUSB



void displayclock_setup()
{
  // Wire.begin();
  // SerialMonitorInterface.begin(9600);

  // display.begin();                            //Initializes TinyScreen board
                       //Flips the TinyScreen rightside up for O Watch
  display.on();                               //Turns TinyScreen display on
  display.fontColor(TS_8b_White,TS_8b_Black); //Set the font color, font background
  display.setBrightness(10);                  //Set display brightness 0 - 15
  
  // Set the time and date. Change this to your current date and time.
  setTime(10,10,10,18,11,2022);    //values in the order hr,min,sec,day,month,year

}

void displayclock_loop()
{

  display.setFont(liberationSansNarrow_12ptFontInfo);   //Set the font type

  // Print date in US format MM:DD:YY (Switch the order in which day, month, year that you like to use)
  display.setCursor(15,8); //Set the cursor where you want to start printing the date
  display.print(monthShortStr(month()));

  display.print(" ");
  display.print(day()); 

  display.print(", ");
  display.print(year());

  display.setCursor(15,25); //Set the cursor where you want to start printing the date
  display.print(dayStr(weekday()));

  display.setFont(liberationSansNarrow_16ptFontInfo);   //Set the font type

  // display time in HH:MM:SS 24 hour format
  display.setCursor(15,45); //Set the cursor where you want to start printing the time
  if(hour()<10) display.print(0); //print a leading 0 if hour value is less than 0
  display.print(hour());
  display.print(":");
  if(minute()<10) display.print(0); //print a leading 0 if minute value is less than 0
  display.print(minute());
  display.print(":");
  if(second()<10) display.print(0); //print a leading 0 if seconds value is less than 0
  display.print(second());
  display.print(" "); //just a empty space after the seconds

}

void printday(){
  display.setFont(liberationSansNarrow_12ptFontInfo);
  display.setCursor(40,35);
  if(day()<10) display.print(0);
  display.print(day()); 
}

void printmonth(){
  display.setFont(liberationSansNarrow_12ptFontInfo);
  display.setCursor(35,35);
  display.print(monthShortStr(month()));
}

void printyear(){
  display.setFont(liberationSansNarrow_12ptFontInfo);
  display.setCursor(30,35);
  display.print(year());
}



void printclock(){
  display.setFont(liberationSansNarrow_16ptFontInfo); 
  display.setCursor(15,35); //Set the cursor where you want to start printing the time
  if(hour()<10) display.print(0); //print a leading 0 if hour value is less than 0
  display.print(hour());
  display.print(":");
  if(minute()<10) display.print(0); //print a leading 0 if minute value is less than 0
  display.print(minute());
  display.print(":");
  if(second()<10) display.print(0); //print a leading 0 if seconds value is less than 0
  display.print(second());
  display.print(" "); //just a empty space after the seconds
}

void printSetTime(){
  display.setFont(liberationSansNarrow_12ptFontInfo); 
  display.setCursor(15,10);
  display.print("Set Time");
}

void printSetDate(){
  display.setFont(liberationSansNarrow_12ptFontInfo); 
  display.setCursor(20,13);
  display.print("Set Date");
}

void printSetMonth(){
  display.setFont(liberationSansNarrow_12ptFontInfo); 
  display.setCursor(20,13);
  display.print("Set Month");
}

void printSetYear(){
  display.setFont(liberationSansNarrow_12ptFontInfo); 
  display.setCursor(20,13);
  display.print("Set Year");
}



void printclockarrow(){
  display.setFont(liberationSansNarrow_10ptFontInfo); 
  display.setCursor(83,50);
  display.print(">>");
  display.setFont(liberationSansNarrow_10ptFontInfo); 
  display.setCursor(85,10);
  display.print(">");
  display.setFont(liberationSansNarrow_10ptFontInfo); 
  display.setCursor(1,10);
  display.print("^");
  display.setFont(liberationSansNarrow_10ptFontInfo); 
  display.setCursor(1,50);
  display.print("v");
  
}

void printhourcursor(){
  display.setFont(liberationSansNarrow_10ptFontInfo);
  display.setCursor(25,27);
  display.print("^");
  display.setCursor(25,50);
  display.print("v");
}

void printminutecursor(){
  display.setFont(liberationSansNarrow_10ptFontInfo);
  display.setCursor(45,27);
  display.print("^");
  display.setCursor(45,50);
  display.print("v");
}

void printsecondcursor(){
  display.setFont(liberationSansNarrow_10ptFontInfo);
  display.setCursor(65,27);
  display.print("^");
  display.setCursor(65,50);
  display.print("v");
}
//to store millis() value when we entered menu page 1
//used to track how long we have been on the menu page 1
//when clock_when_on_menu = 0, it means we have never spent any time on the menu page 1
//however, when clock_when_on_menu is given any value, it means that we have been on menu page 1 since the time stored on the variable
long clock_when_on_menu = 0;
uint8_t clock_update(){
  long current_millis= millis();
  if(clock_millis>current_millis){
    clock_millis=0;
  }

  if(menu_page_number==1){
    if(current_millis-clock_millis >= 1000 || clock_when_on_menu == 0) {
      displayclock_loop();
      ble_quick_draw(1,50,"<", 1);
      clock_millis=millis();
      if (clock_when_on_menu == 0){clock_when_on_menu = millis();}
    }
    if(display.getButtons(TSButtonLowerLeft) && clock_when_on_menu > 0 && abs(current_millis-clock_when_on_menu > 350)){
      display.clearScreen();
      printSetTime();
      printclockarrow();
      printhourcursor();
      menu_page_number = 2;
      set_time_track = 0;
      clock_millis = millis(); //put a delay on btm left button, so it doesn't instantly change hour value
    }
  }
  else {clock_when_on_menu = 0;}
  
  if(menu_page_number==2){
    if(current_millis-clock_millis >= 150) {
    

      if(display.getButtons(TSButtonLowerRight)){
        display.clearScreen();
        set_time_track=-1;
        menu_page_number = 1;

        //to do: implement ble_when_on_menu instead (keep track of how long we have been on the menu on ble side instead)
        ble_command_timer = millis();
      }

      if(display.getButtons(TSButtonUpperRight)){
        
        set_time_track++;
        if(set_time_track>5){
          set_time_track=0;
        }
        if (set_time_track==0){
        display.clearScreen();
        printSetTime();
        printclockarrow();
        printhourcursor();
        }
        if (set_time_track==1){
        display.clearWindow(15,27,65,50);
        printclock();
        printminutecursor();
        }
        if (set_time_track==2){
        display.clearWindow(15,27,65,50);
        printclock();
        printsecondcursor();
      }
        if (set_time_track==3){
        display.clearScreen(); 
        printminutecursor();
        printclockarrow();
        printSetDate();
        printday();
        }

        if (set_time_track==4){
        display.clearWindow(15,27,65,34);
        printSetMonth();
        printmonth();
        printminutecursor();
        }

        if (set_time_track==5){
        display.clearScreen();
        printSetYear();
        printyear();
        printminutecursor();
        printclockarrow();

        }
      }

      if(set_time_track == 0){
        printclock();
        int hr = hour();
        if(display.getButtons(TSButtonLowerLeft)){
            if(hr>0){
            hr--;          
            }
            else{
            hr=23;
            }
          }
          if(display.getButtons(TSButtonUpperLeft)){
            hr++;
            if(hr==24){
              hr=0;
            }
          }
          setTime(hr,minute(),second(),day(),month(),year());
      }
      else if(set_time_track == 1){
        printclock();
        int min = minute();
        if(display.getButtons(TSButtonUpperLeft)){
          min++;
          if(min==60){
            min=0;
          }
        }
        if(display.getButtons(TSButtonLowerLeft)){
          if(min>0){
            min--;
          }
          else{
            min=59;
          }
        }
        setTime(hour(),min,second(),day(),month(),year());
      }
      else if(set_time_track == 2){
        printclock();
        int sec = second();
        if(display.getButtons(TSButtonUpperLeft)){
          sec++;
          if(sec > 59){
            sec=0;
          }
        }
        if(display.getButtons(TSButtonLowerLeft)){
            sec--;
            if(sec<0){
              sec=59;
            }
        }
        setTime(hour(),minute(),sec,day(),month(),year());
        }
      else if(set_time_track == 3){
        printday();
        int date = day();
        if(display.getButtons(TSButtonUpperLeft)){
          date++;
        }
        if(display.getButtons(TSButtonLowerLeft)){
          date--;
        }
      setTime(hour(),minute(),second(),date,month(),year());
      }
      else if(set_time_track == 4){
        printmonth();
        int change_month = month();
        if(display.getButtons(TSButtonUpperLeft)){
          change_month++;
          if(change_month > 12){
            change_month = 1;
          }
        }
        if(display.getButtons(TSButtonLowerLeft)){
          change_month--;
          if(change_month < 1){
            change_month = 12;
          }
        }
        setTime(hour(),minute(),second(),day(),change_month,year());
      }
      else if(set_time_track == 5){
        printyear();
        int change_year = year();
        if(display.getButtons(TSButtonUpperLeft)){
          change_year++;
        }
        if(display.getButtons(TSButtonLowerLeft)){
          change_year--;
        }
        setTime(hour(),minute(),second(),day(),month(),change_year);
      }
      clock_millis=millis();
    }
  }
}




#define WHITE 0xFF
#define BLACK 0x00

#include <TinyScreen.h>
#include <Wire.h>
#include <SPI.h>

TinyScreen display = TinyScreen(0);

//true - we are running the Menu screen
//fasle - we are not running Menu anymore 
bool isMain = true; 

//void (*menuHandler) = NULL;

void setup() {
  // put your setup code here, to run once:
  Wire.begin(); //needed to initialize connection
  display.begin(); //should reset the display as well
  drawScreen();
}

void loop() {
    // put your main code here, to run repeatedly:
      //mainProgram();
      //drawScreen(); //we don't need to draw screen repeatedly
}



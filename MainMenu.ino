//Purpose: detect keypress and update screen if necessary


void drawScreen(){
  display.setFont(liberationSans_10ptFontInfo);
  display.fontColor(WHITE,BLACK);
  display.setCursor(0,0);
  display.print("Main Menu");
}

//you can use a class structure, but it must be declared before the setup() function.
//hence, the following won't work unless you move it above
/*
class MainMenu {
  void init() {

  }
  void drawScreen(){
    display.setFont(liberationSans_10ptFontInfo);
    display.fontColor(WHITE,BLACK);
    display.setCursor(0,0);
    display.print("Main menu");
  }
  void loop(){}
};
*/

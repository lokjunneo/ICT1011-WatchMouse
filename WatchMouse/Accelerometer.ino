long accel_millis = 0;
 void accelerometer_mouse(){
  long curr_millis = millis();
  if (accel_millis > curr_millis) {accel_millis = 0;} //in case of overflow
  if (curr_millis - accel_millis > 100){
    accel_sensor.read();//This function gets new data from the acccelerometer

    // Get the acceleration values from the sensor and store them into global variables
    // (Makes reading the rest of the program easier)
    x = accel_sensor.X;
    y = accel_sensor.Y;
    z = accel_sensor.Z;
    //temp = ((accel_sensor.rawTemp * 0.5) + 24.0);

    // If the BMA250 is not found, nor connected correctly, these values will be produced
    // by the sensor 
    if (x == -1 && y == -1 && z == -1) {
      // Print error message to Serial Monitor
      SerialMonitorInterface.print("ERROR! NO BMA250 DETECTED!");
    }
    
    else { // if we have correct sensor readings: 
              int stay = 0;
              int multiple=6;
              int tempx,tempy;
      if(x<-206){
      tempx = -5*multiple;
      }
      else if(x<-156){
        tempx=-4*multiple;
      }
      else if(x<-106){
        tempx=-3*multiple;
      }
      else if(x<-56){
        tempx=-2*multiple;
      }
      else if(x>=-56&&x<=56){
        tempx=stay;
      }
      else if(x>56){
        tempx= 2*multiple;
      }
      else if(x>106){
        tempx= 3*multiple;
      }
        else if(x>156){
        tempx= 4*multiple;
      }
      else if(x>206){
        tempx = 5*multiple;
      }



      if(y<-206){
        tempy=-5*multiple;
      }
      else if(y<-156){
        tempy=-4*multiple;
      }
      else if(y<-106){
        tempy=-3*multiple;
      }
      else if(y<-56){
        tempy=-2*multiple;
      }
      else if(y>=-56&&y<=56){
        tempy=stay;
      }
      else if(y>56){
        tempy=2*multiple;
      }
      else if(y>106){
        tempy=3*multiple;
      }
      else if(y>156){
        tempy=4*multiple;
      }
      else if(y>206){
        tempy=5*multiple;
      }
      updateMouseXY(tempy,tempx);
      accel_millis = millis();
    }
    // The BMA250 can only poll new sensor values every 64ms, so this delay
    // will ensure that we can continue to read values
  }
  // ***Without the delay, there would not be any sensor output*** 
}
/*
uint8_t updateMouseXY(int8_t offsetX,int8_t offsetY){
  
  int8_t currX = (int8_t)curr_mouse_report[2];
  int8_t currY = (int8_t)curr_mouse_report[1];

  int8_t newX = offsetX + currX;
  int8_t newY = offsetY*-1 + currY;
  //overflow: -ve + -ve give positive
  if (offsetX < 0 && currX < 0 && newX > 0){ newX = -128;}
  //overflow: +ve + +ve give negative
  else if (offsetX > 0 && currX > 0 && newX < 0) {newX = 127;}

  if (offsetY < 0 && currY < 0 && newY > 0){ newY = -128;}
  else if (offsetY > 0 && currY > 0 && newY < 0) {newY = 127;}

  curr_mouse_report[2] = newX;
  curr_mouse_report[1] = newY;

  //ble_call_update = 40;
}*/
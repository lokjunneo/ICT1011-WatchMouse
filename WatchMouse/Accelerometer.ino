
int tempx = 0,tempy = 0;
 void accelerometer_mouse(){
    accel_sensor.read();//This function gets new data from the acccelerometer

    // Get the acceleration values from the sensor and store them into global variables
    // (Makes reading the rest of the program easier)
    x = accel_sensor.X;
    y = accel_sensor.Y;
    //temp = ((accel_sensor.rawTemp * 0.5) + 24.0);

    // If the BMA250 is not found, nor connected correctly, these values will be produced
    // by the sensor 
    if (x == -1 && y == -1) {
      // Print error message to Serial Monitor
      SerialMonitorInterface.print("ERROR! NO BMA250 DETECTED!");
    }
    
    else { // if we have correct sensor readings: 
              int multiple=10;
              int tempx,tempy;
      if(x<-56){
      tempx = -5*multiple;
      }
      else if(x>=-56&&x<=56){
        tempx=0;
      }
      else if(x>56){
        tempx = 5*multiple;
      }



      if(y<-56){
        tempy=-5*multiple;
      }
      else if(y>=-56&&y<=56){
        tempy=0;
      }
      else if(y>56){
        tempy=5*multiple;
      }
      updateMouseXY(tempy*-1,tempx);
    }
    // The BMA250 can only poll new sensor values every 64ms, so this delay
    // will ensure that we can continue to read values
  }
  // ***Without the delay, there would not be any sensor output*** 


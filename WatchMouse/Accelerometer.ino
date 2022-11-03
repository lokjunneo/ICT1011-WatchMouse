
int x, y, z;
int tempx = 0,tempy = 0;
 void accelerometer_mouse(){
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
      
     if(x<-206){
    tempx = -10;
    }
    else if(x<-156){
      tempx=-5;
    }
    else if(x<-130){
      tempx=-3;
    }
    else if(x<-80){
      tempx=-1;
    }
    else if(x>=-80&&x<=80){
      tempx=0;
    }
     else if(x>80){
      tempx= 1;
    }
     else if(x>130){
      tempx= 3;
    }
      else if(x>156){
      tempx= 5;
    }
    else if(x>206){
      tempx = 10;
    }

        if(y<-206){
      tempy=-10;
    }
    else if(y<-156){
      tempy=-5;
    }
    else if(y<-106){
      tempy=-3;
    }
    else if(y<-36){
      tempy=-1;
    }
    else if(y>=-36&&y<=36){
      tempy=0;
    }
    else if(y>36){
      tempy=1;
    }
    else if(y>106){
      tempy=3;
    }
    else if(y>156){
      tempy=5;
    }
    else if(y>206){
      tempy=10;
    }

      updateMouseXY(tempy,tempx);
    }
    // The BMA250 can only poll new sensor values every 64ms, so this delay
    // will ensure that we can continue to read values
  }
  // ***Without the delay, there would not be any sensor output*** 


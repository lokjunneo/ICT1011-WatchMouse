
int16_t x=0, y=0, z=0;
int tempx = 0,tempy = 0;
long accel_millis = 0; //to delay when to read from sensor; 
long update_millis = 0; //to delay updateMouse
void accelerometer_mouse(){
long curr_millis = millis();

  /* In case of overflow */
  if (update_millis > curr_millis){
    update_millis = 0;
    accel_millis = 0;
  }
  /* BMA only updates value every 70ms */
  if (curr_millis - accel_millis > 70){
    accel_sensor.read();//This function gets new data from the acccelerometer
    x = accel_sensor.X;
    y = accel_sensor.Y;
    z = accel_sensor.Z;
    accel_millis = millis();
  }
  if ( curr_millis - update_millis > 10){
    /*  Make sure BMA250 remains detected */
    if (x == -1 && y == -1 && z == -1) {
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
      else if(x>206){
        tempx = 10;
      }
      else if(x>156){
        tempx= 5;
      }
      else if(x>130){
        tempx= 3;
      }
      else if(x>80){
        tempx= 1;
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
      else if(y>206){
        tempy=10;
      }
      else if(y>156){
        tempy=5;
      }
      else if(y>106){
        tempy=3;
      }
      else if(y>36){
        tempy=1;
      }


/*
      SerialMonitorInterface.print("x = ");  
      SerialMonitorInterface.print(x);
      SerialMonitorInterface.print(", y = ");  
      SerialMonitorInterface.print(y);
      SerialMonitorInterface.print(", z = ");  
      SerialMonitorInterface.println(z);*/
      updateMouseXY(tempy,tempx);
    }
    update_millis = millis();
  }
}
  // ***Without the delay, there would not be any sensor output*** 


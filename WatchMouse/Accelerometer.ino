
int16_t accel_x=0, accel_y=0, accel_z=0;
int tempx = 0,tempy = 0;
long accel_millis = 0; //to delay when to read from sensor; 
long update_millis = 0; //to delay updateMouse

int accel_cal_x = 0, accel_cal_y = 0; //For storing calibration data of user

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
    accel_x = accel_sensor.X;
    accel_y = accel_sensor.Y;
    accel_z = accel_sensor.Z;
    accel_millis = millis();
  }
  // Slows down update a little, which helps makes cursor movements feel smoother
  if ( curr_millis - update_millis > 10){
    /*  Make sure BMA250 remains detected */
    if (accel_x == -1 && accel_y == -1 && accel_z == -1) {
      SerialMonitorInterface.print("ERROR! NO BMA250 DETECTED!");
    }
    
    else { // if we have correct sensor readings: 
        
      if(accel_x<-206+accel_cal_x){
        tempx = -10;
      }
      else if(accel_x<-156+accel_cal_x){
        tempx=-5;
      }
      else if(accel_x<-130+accel_cal_x){
        tempx=-3;
      }
      else if(accel_x<-80+accel_cal_x){
        tempx=-1;
      }
      else if(accel_x>=-80+accel_cal_x && accel_x<=80+accel_cal_x){
        tempx=0;
      }
      else if(accel_x>206+accel_cal_x){
        tempx = 10;
      }
      else if(accel_x>156+accel_cal_x){
        tempx= 5;
      }
      else if(accel_x>130+accel_cal_x){
        tempx= 3;
      }
      else if(accel_x>80+accel_cal_x){
        tempx= 1;
      }

      if(accel_y<-206+accel_cal_y){
        tempy=-10;
      }
      else if(accel_y<-156+accel_cal_y){
        tempy=-5;
      }
      else if(accel_y<-106+accel_cal_y){
        tempy=-3;
      }
      else if(accel_y<-36+accel_cal_y){
        tempy=-1;
      }
      else if(accel_y>=-36+accel_cal_y && accel_y<=36 + accel_cal_y){
        tempy=0;
      }
      else if(accel_y>206 + accel_cal_y){
        tempy=10;
      }
      else if(accel_y>156 + accel_cal_y){
        tempy=5;
      }
      else if(accel_y>106 + accel_cal_y){
        tempy=3;
      }
      else if(accel_y>36 + accel_cal_y){
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


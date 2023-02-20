#include <Wire.h>

int ADXL345 = 0x53;
float x, y, z;
float x_list [10], y_list [10], z_list [10];
int index = 0;

void setup(){
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(ADXL345);
  Wire.write(0x2D);
  Wire.write(8);
  Wire.write(0x31);
  Wire.write(0b10);
  Wire.endTransmission();
}

void loop() {
  int j = 1;

  Wire.beginTransmission(ADXL345);
  Wire.write(0x32);  // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true);    // Read 6 registers total, each axis value is stored in 2 registers
  x = (Wire.read() | Wire.read() << 8);  // X-axis value
  //x = x/256; 
  y = (Wire.read() | Wire.read() << 8);  // Y-axis value
  //y = y/256;
  z = (Wire.read() | Wire.read() << 8);  // Z-axis value
  //z = z/256;

  

  if(index < 10){
    x_list[index] = x;
    y_list[index] = y;
    z_list[index] = z;
  }

  else{
    for(int i = 0; i < 10; i++){

      if(j < 10){
        x_list[i] = x_list[j];
        y_list[i] = y_list[j];
        z_list[i] = z_list[j];
      }

      else if(j == 10){
        x_list[9] = x;
        y_list[9] = y;
        z_list[9] = z;
      }

      j++;
    }
  }

  index++;

  if(index > 10){
    Serial.println("Z list: ");
  
    for(int i; i < 10; i++){
      Serial.print(z_list[i]);
      Serial.print(" ");
    }
  }
  
  delay(100);
}

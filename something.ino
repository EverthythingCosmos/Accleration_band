#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

int ADXL345 = 0x53;  // The ADXL345 sensor I2C address

float x, y, z;  // Outputs
float prev_z;
float error;

void setup() {
  Serial.begin(9600);  // Initiate serial communication for printing the results on the Serial monitor
  Wire.begin();        // Initiate the Wire library
  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345);  // Start communicating with the device
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  // Enable measurement
  
  Wire.write(8);  // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable
  Wire.endTransmission();
  delay(10);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() {
  // === Read acceleromter data === //
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32);  // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true);    // Read 6 registers total, each axis value is stored in 2 registers
  x = (Wire.read() | Wire.read() << 8);  // X-axis value
  //x = x/256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
  y = (Wire.read() | Wire.read() << 8);  // Y-axis value
  //y = y/256;
  z = (Wire.read() | Wire.read() << 8);  // Z-axis value
  //z = z/256;

  error = z - prev_z;
  //Serial.println(error);

  // if(z > 0){
  //   digitalWrite(13, HIGH);
  // }

  // else{
  //   digitalWrite(13, LOW);
  // }

  if (z > 0 && error > 500 && x > !-300 && y < -100) {
    Serial.println("Positive Acceleration detected");
    digitalWrite(13, HIGH);
  }

  else if (z < 0 && error < -500) {
    digitalWrite(13, LOW);
  }

  Serial.print("Xa= ");
  Serial.print(x);
  Serial.print("   Ya= ");
  Serial.print(y);
  Serial.print("   Za= ");
  Serial.println(z);
}
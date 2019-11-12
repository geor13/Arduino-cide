#include <Wire.h>  // Wire library - used for I2C communication
#include<SoftwareSerial.h>
int ADXL345 = 0x53; // The ADXL345 sensor I2C address
float X_out, Y_out, Z_out;  // Outputs
float roll,pitch,rollF,pitchF=0;

int timecounter;

int maxRoll = 0;
int maxPinch = 0;

float mappedRoll;
float mappedPitch;

SoftwareSerial bluetooth(10,9);


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); // Initiate serial communication for printing the results on the Serial monitor
 
  Wire.begin(); // Initiate the Wire library
  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345); // Start communicating with the device
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  // Enable measurement
  Wire.write(8); // Bit D3 High for measuring enable (8dec -> 0000 1000 binary)
  Wire.endTransmission();
  delay(10);
  //Off-set Calibration
  //X-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1E);
  Wire.write(1);
  Wire.endTransmission();
  delay(10);
  //Y-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1F);
  Wire.write(-2);
  Wire.endTransmission();
  delay(10);
  //Z-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x20);
  Wire.write(-9);
  Wire.endTransmission();
  delay(10);

  bluetooth.begin(9600);



}

void loop() {
  // put your main code here, to run repeatedly:
  
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  X_out = ( Wire.read() | Wire.read() << 8); // X-axis value
  X_out = X_out / 256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
  Y_out = ( Wire.read() | Wire.read() << 8); // Y-axis value
  Y_out = Y_out / 256;
  Z_out = ( Wire.read() | Wire.read() << 8); // Z-axis value
  Z_out = Z_out / 256;

  //Serial.println(Y_out);
  // Calculate Roll and Pitch (rotation around X-axis, rotation around Y-axis)
  roll = atan(Y_out / sqrt(pow(X_out, 2) + pow(Z_out, 2))) * 180 / PI;
  pitch = atan(-1 * X_out / sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;
  // Low-pass filter
  rollF = 0.94 * rollF + 0.06 * roll;
  pitchF = 0.94 * pitchF + 0.06 * pitch;


  
  /*
  //SOS!!! TA XREIAZOMAI AUTA !!!
  bluetooth.print(rollF); //παει στην εφαρμογή του κινητου
  bluetooth.print("/");   //παει στην εφαρμογή του κινητου
  bluetooth.print(pitchF);//παει στην εφαρμογή του κινητου
  bluetooth.print("/");
  bluetooth.println(Z_out);

  */

  bluetooth.print(X_out); //παει στην εφαρμογή του κινητου

  
  bluetooth.print("/");   //παει στην εφαρμογή του κινητου
  
  bluetooth.print(Y_out);//παει στην εφαρμογή του κινητου
  bluetooth.print("/");
  bluetooth.print(Z_out);
  bluetooth.println(" ");  //TESTING!!!!!  ///SOOOOS μπορει το ln να μην κανει !!!!
 // bluetooth.print(Z_out);
 // bluetooth.print("/");
 // bluetooth.println(Z_out);

    Serial.print(X_out);
    Serial.print(" / ");
    Serial.print(Y_out);
    Serial.print(" / ");
    Serial.print(Z_out);
    Serial.print(" / ");
   Serial.print(roll); //εκτυπωνω για αν δω τα maxRoll και maxPitch
   Serial.print(" / ");
    Serial.println(pitch);
  // Serial.print(" / ");
   
   

  if(bluetooth.available())
  {
    Serial.print("is available");
    Serial.write(bluetooth.read());
  }


 delay(10);


}

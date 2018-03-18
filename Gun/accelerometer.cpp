#include "communication.hpp"
#include "accelerometer.hpp"
#include <math.h>
#include <Wire.h>
#include <Arduino.h>
#define min(a,b) ((a)<=(b)?(a):(b))
#define max(a,b) ((a)>=(b)?(a):(b))


const float Accelerometer::AccelScaleFactor = 16384;
int16_t Accelerometer::AccelX = 0, Accelerometer::AccelY=0, Accelerometer::AccelZ=0;
int16_t Accelerometer::GyroX = 0, Accelerometer::GyroY=0, Accelerometer::GyroZ=0;
float Accelerometer::Pitch = 0,Accelerometer::Roll = 0;
const int n = 1;
float measureX [n] = {};
float measureY [n] = {};
float measureZ [n] = {};

void przesun(float * tab,int _size){
  for (int i = 0; i<_size-1;i++){
    tab[i] = tab[i+1];
  }
  tab[_size-1] = 0;
}
float sum(float * tab,int _size){
  float suma = 0;
  for (int i = 0; i<_size;i++)
    suma+=tab[i];
  return suma;
}
void Accelerometer::process_accelerometer(){
  Accelerometer::Read_RawValue();
 /* Communication::Send_Number(40,sum(measureX,n)/n);
  Communication::Send_Number(41,sum(measureY,n)/n);
  Communication::Send_Number(42,sum(measureZ,n)/n);
  /*Communication::Send_Number(40,Accelerometer::get_acc_degrees(Accelerometer::AccelX));
  Communication::Send_Number(41,Accelerometer::get_acc_degrees(Accelerometer::AccelY));
  Communication::Send_Number(42,Accelerometer::get_acc_degrees(Accelerometer::AccelZ));*/
}

float Accelerometer::get_acc_degrees(float raw_value){
  float tmp = raw_value/Accelerometer::AccelScaleFactor;
  tmp = max(tmp,-1.0f);
  tmp = min(tmp,1.0f);
  return acos(tmp)*180.0f/3.14f;
}

void Accelerometer::I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// read all 14 register
void Accelerometer::Read_RawValue(){
  uint8_t deviceAddress = Accelerometer::MPU6050SlaveAddress;
  uint8_t regAddress = Accelerometer::MPU6050_REGISTER_ACCEL_XOUT_H;
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);
  Accelerometer::AccelX = (((int16_t)Wire.read()<<8) | Wire.read());
  Accelerometer::AccelY = (((int16_t)Wire.read()<<8) | Wire.read());
  Accelerometer::AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
  (((int16_t)Wire.read()<<8) | Wire.read());
  Accelerometer::GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
  Accelerometer::GyroY = (((int16_t)Wire.read()<<8) | Wire.read());
  Accelerometer::GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
}

//configure MPU6050
void Accelerometer::init(){
  delay(150);
  Accelerometer::I2C_Write(Accelerometer::MPU6050SlaveAddress, Accelerometer::MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  Accelerometer::I2C_Write(Accelerometer::MPU6050SlaveAddress,Accelerometer:: MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  Accelerometer::I2C_Write(Accelerometer::MPU6050SlaveAddress, Accelerometer::MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  Accelerometer::I2C_Write(Accelerometer::MPU6050SlaveAddress, Accelerometer::MPU6050_REGISTER_CONFIG, 0x00);
  Accelerometer::I2C_Write(Accelerometer::MPU6050SlaveAddress, Accelerometer::MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
  Accelerometer::I2C_Write(Accelerometer::MPU6050SlaveAddress, Accelerometer::MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
  Accelerometer::I2C_Write(Accelerometer::MPU6050SlaveAddress, Accelerometer::MPU6050_REGISTER_FIFO_EN, 0x00);
  Accelerometer::I2C_Write(Accelerometer::MPU6050SlaveAddress, Accelerometer::MPU6050_REGISTER_INT_ENABLE, 0x01);
  Accelerometer::I2C_Write(Accelerometer::MPU6050SlaveAddress, Accelerometer::MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  Accelerometer::I2C_Write(Accelerometer::MPU6050SlaveAddress, Accelerometer::MPU6050_REGISTER_USER_CTRL, 0x00);
}

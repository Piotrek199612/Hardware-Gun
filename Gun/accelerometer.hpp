#ifndef accelerometer_hpp
#define accelerometer_hpp

#include <stdint.h>
void przesun(float * tab,int _size);
float sum(float * tab,int _size);

class Accelerometer{
  static const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
  static const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
  static const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
  static const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
  static const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
  static const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
  static const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
  static const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
  static const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
  static const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
  static const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;
  static const uint8_t MPU6050SlaveAddress = 0x68;

  
  static const float AccelScaleFactor;
  
  
  static void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data);
public:  
  static void Read_RawValue();
  static int16_t AccelX, AccelY, AccelZ;
  static int16_t GyroX, GyroY, GyroZ;
  
  static float get_acc_degrees(float raw_value); 
  
  public:
  static float Pitch , Roll;
  static void init();
  static void process_accelerometer();
};
#endif accelerometer_hpp


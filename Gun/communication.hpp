#ifndef communication_hpp
#define communication_hpp

#include <stdint.h>

class Communication{
  public:
    static const int WIFI_CHANNEL = 4;
    static uint8_t remoteMac [];
    static void init();
    static void Send(uint8_t data[],uint8_t data_length);
    static void Send(uint8_t id,uint8_t decimal,uint8_t fraction,uint8_t data_length);
    static void Send_Number(uint8_t id,float number);
};

#endif communication_hpp

/*
 * Communication id:
 * 40 - Accelerometer X
 * 41 - Accelerometer Y
 * 42 - Accelerometer Z
 * 1 - Grip 0 button
 * 2 - Grip 1 button
 * 3 - Grip 2 button
 * 4 - Grip 3 button
 * 5 - Grip 4 button
 * 6 - Trigger button
 * 7 - Aim button
 */

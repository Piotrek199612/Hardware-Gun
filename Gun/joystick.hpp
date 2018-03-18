#ifndef joystick_hpp
#define joystick_hpp

#include <stdint.h>
#include <pins_arduino.h>
class Joystick{
  public:
    static void init();
    static void processButtons();
    
   private:
    static const uint8_t ADDRESS_0 = D3;
    static const uint8_t ADDRESS_1 = D4;
    static const uint8_t ADDRESS_2 = D5;
    static const uint8_t ANALOG_INPUT = A0;

    static const uint8_t X_ADDRESS = 0;
    static const uint8_t Y_ADDRESS = 1;

    static const uint8_t X_MSGID = 80;
    static const uint8_t Y_MSGID = 81;

    static float x_last_value;
    static float y_last_value;

    static void set_address(uint8_t address);
    static float read_axis(uint8_t address);
};
#endif joystick_hpp

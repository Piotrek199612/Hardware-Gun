#ifndef controller_hpp
#define controller_hpp

#include <stdint.h>
#include "MYPCF8574.h"

class Controller{
  public:
    static void init();

    static bool readTrigger();
    static bool readAim();
    static bool readGrip(uint8_t number);
    static void processButtons();

    
    static PCF8574* expander;
  //Ekspander adres 0x20
  //Joystick adres 0x68
  
    static bool grip_0_last_value;
    static bool grip_1_last_value;
    static bool grip_2_last_value;
    static bool grip_3_last_value;
    static bool grip_4_last_value;
    static bool grip_5_last_value;
    static bool trigger_last_value;
    static bool aim_last_value;
  
    static const uint8_t GRIP_0 = 1; // ekspander
    static const uint8_t GRIP_1 = 2; // ekspander
    static const uint8_t GRIP_2 = 3; // ekspander
    static const uint8_t GRIP_3 = 4; // ekspander
    static const uint8_t GRIP_4 = 5; // ekspander
    static const uint8_t GRIP_5 = 0; // ekspander
    static const uint8_t TRIGGER = 6; // ekspander
    static const uint8_t AIM = 7; // ekspander

};
#endif controller_hpp


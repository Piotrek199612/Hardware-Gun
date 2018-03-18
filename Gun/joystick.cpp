#include "joystick.hpp"
#include "communication.hpp"

#include <pins_arduino.h>
#include <arduino.h>

float Joystick::x_last_value = 0;
float Joystick::y_last_value = 0;

void Joystick::init(){
  pinMode(Joystick::ADDRESS_0,OUTPUT);
  pinMode(Joystick::ADDRESS_1,OUTPUT);
  pinMode(Joystick::ADDRESS_2,OUTPUT);

  pinMode(Joystick::ANALOG_INPUT,INPUT);

}

void Joystick::set_address(uint8_t address){
  digitalWrite(Joystick::ADDRESS_0,address&1);
  digitalWrite(Joystick::ADDRESS_1,address&(1<<1));
  digitalWrite(Joystick::ADDRESS_2,address&(1<<2));
}

void Joystick::processButtons(){
  float tmp;
  tmp = read_axis(Joystick::X_ADDRESS);
  if (tmp != x_last_value){
    x_last_value = tmp;
    Communication::Send_Number(X_MSGID,tmp);
  }

  tmp = read_axis(Joystick::Y_ADDRESS);
  if (tmp != y_last_value){
    y_last_value = tmp;
    Communication::Send_Number(Y_MSGID,tmp);
  }
}

float Joystick::read_axis(uint8_t address){
   Joystick::set_address(address);
   return analogRead(ANALOG_INPUT);
}

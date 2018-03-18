#include "controller.hpp"
#include "communication.hpp"

PCF8574* Controller::expander = new PCF8574();
bool Controller::grip_0_last_value = true;
bool Controller::grip_1_last_value = true;
bool Controller::grip_2_last_value = true;
bool Controller::grip_3_last_value = true;
bool Controller::grip_4_last_value = true;
bool Controller::grip_5_last_value = true;
bool Controller::trigger_last_value = true;
bool Controller::aim_last_value = true;

void Controller::init(){
  Controller::expander->begin(0x20);
  for (int i = 0; i < 8; i++)
    expander->pinMode(i, INPUT);

  grip_0_last_value = readGrip(0);
  grip_1_last_value = readGrip(1);
  grip_2_last_value = readGrip(2);
  grip_3_last_value = readGrip(3);
  grip_4_last_value = readGrip(4);
  grip_5_last_value = readGrip(5);
  trigger_last_value = readTrigger;
  aim_last_value = readAim();
}

bool Controller::readTrigger(){
  return expander->digitalRead(TRIGGER);
}
bool Controller::readAim(){
  return expander->digitalRead(AIM);
}
bool Controller::readGrip(uint8_t number){
  switch (number){
    case 0: return expander->digitalRead(GRIP_0);
    case 1: return expander->digitalRead(GRIP_1);
    case 2: return expander->digitalRead(GRIP_2);
    case 3: return expander->digitalRead(GRIP_3);
    case 4: return expander->digitalRead(GRIP_4);
    case 5: return expander->digitalRead(GRIP_5);
  }
}

void Controller::processButtons(){
  float tmp;
  tmp = readGrip(0);
  if (tmp != grip_0_last_value){
    grip_0_last_value = tmp;
    Communication::Send_Number(0,tmp);
  }
  tmp = readGrip(1);
  if (tmp != grip_1_last_value){
    grip_1_last_value = tmp;
    Communication::Send_Number(1,tmp);
  }
  tmp = readGrip(2);
  if (tmp != grip_2_last_value){
    grip_2_last_value = tmp;
    Communication::Send_Number(2,tmp);
  }
  tmp = readGrip(3);
  if (tmp != grip_3_last_value){
    grip_3_last_value = tmp;
    Communication::Send_Number(3,tmp);
  }
  tmp = readGrip(4);
  if (tmp != grip_4_last_value){
    grip_4_last_value = tmp;
    Communication::Send_Number(4,tmp);
  }
  tmp = readGrip(5);
  if (tmp != grip_5_last_value){
    grip_5_last_value = tmp;
    Communication::Send_Number(5,tmp);
  }
  tmp = readTrigger();
  if (tmp != trigger_last_value){
    trigger_last_value = tmp;
    Communication::Send_Number(6,tmp);
  }
  tmp = readAim();
  if (tmp != aim_last_value){
    aim_last_value = tmp;
    Communication::Send_Number(7,tmp);
  }
}

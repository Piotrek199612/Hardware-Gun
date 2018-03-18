/*
   See header file for details

    This program is free software: you can redistribute it and/or modify\n
    it under the terms of the GNU General Public License as published by\n
    the Free Software Foundation, either version 3 of the License, or\n
    (at your option) any later version.\n

    This program is distributed in the hope that it will be useful,\n
    but WITHOUT ANY WARRANTY; without even the implied warranty of\n
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n
    GNU General Public License for more details.\n

    You should have received a copy of the GNU General Public License\n
    along with this program.  If not, see <http://www.gnu.org/licenses/>.\n
*/

/* Dependencies */
#include <Wire.h>
#include "MYPCF8574.h"

PCF8574::PCF8574() :
  _OUTPUTS(0), _INPUTS(0), _PINMODES(0), _address(0)

{
}

void PCF8574::begin(uint8_t address) {

  /* Store the I2C address and init the Wire library */
  _address = address;
  Wire.begin();
  beginGPIO();
}

void PCF8574::pinMode(uint8_t pin, uint8_t mode) {

  /* Switch according mode */
  switch (mode) {
    case INPUT:
      _PINMODES &= ~(1 << pin);
      break;

    case INPUT_PULLUP:
      _PINMODES &= ~(1 << pin);
      break;

    case OUTPUT:
      _PINMODES |= (1 << pin);
      break;

    default:
      break;
  }
}

void PCF8574::digitalWrite(uint8_t pin, uint8_t value) {

  /* Set PORT bit value */
  if (value)
    _OUTPUTS |= (1 << pin);
  else
    _OUTPUTS &= ~(1 << pin);

  /* Update GPIO values */
  updateGPIO();
}

uint8_t PCF8574::digitalRead(uint8_t pin) {

  /* Read GPIO */
  readGPIO();

  /* Read and return the pin state */
  return (_INPUTS & (1 << pin)) ? HIGH : LOW;
}

void PCF8574::write(uint8_t value) {

  /* Store pins values and apply */
  _OUTPUTS = value;

  /* Update GPIO values */
  updateGPIO();
}

uint8_t PCF8574::read() {

  /* Read GPIO */
  readGPIO();

  /* Return current pins values */
  return _INPUTS;
}

void PCF8574::pullUp(uint8_t pin) {

  /* Same as pinMode(INPUT_PULLUP) */
  pinMode(pin, INPUT_PULLUP); // /!\ pinMode form THE LIBRARY
}

void PCF8574::pullDown(uint8_t pin) {

  /* Same as pinMode(INPUT) */
  pinMode(pin, INPUT); // /!\ pinMode form THE LIBRARY
}

void PCF8574::clear() {

  /* User friendly wrapper for write() */
  _OUTPUTS = 0b00000000;
  updateGPIO();
}

void PCF8574::set() {

  /* User friendly wrapper for write() */
  _OUTPUTS = 0b11111111;
  updateGPIO();

}

void PCF8574::toggle(uint8_t pin) {

  /* Toggle pin state */
  _OUTPUTS ^= (1 << pin);

  /* Update GPIO values */
  updateGPIO();
}

void PCF8574::blink(uint8_t pin, uint16_t count, uint32_t duration) {

  /* Compute steps duration */
  duration /= count * 2;

  /* Loop n times */
  while (count--) {

    /* Toggle pin 2 times */
    toggle(pin);
    delay(duration);
    toggle(pin);
    delay(duration);
  }
}


void PCF8574::readGPIO() {
  Wire.requestFrom(_address, (uint8_t) 1);
  while (Wire.available())_INPUTS = Wire.read();
}

void PCF8574::updateGPIO() {
  readGPIO();
  uint8_t values = (_INPUTS & not _PINMODES) | (_OUTPUTS & _PINMODES);
  /* Start communication and send GPIO values as byte */
  Wire.beginTransmission(_address);
  Wire.write(values);
  Wire.endTransmission();
}
void PCF8574::beginGPIO() {
  /* Start communication and send GPIO values as byte */
  Wire.beginTransmission(_address);
  Wire.write(0);
  Wire.endTransmission();
}

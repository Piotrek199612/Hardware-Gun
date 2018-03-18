/*
HMC5883L.cpp - Class file for the HMC5883L Triple Axis Digital Compass Arduino Library.

Version: 1.1.0
(c) 2014 Korneliusz Jarzebski
www.jarzebski.pl

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Wire.h>
#include <arduino.h>
#include "magnetometer.h"
#include "communication.hpp"
#include "accelerometer.hpp"

const int n = 4;
float measureDeg [n] = {};

int Magnetometer::xOffset = 0;
int Magnetometer::yOffset = 0;
int Magnetometer::zOffset = 0;
Vector Magnetometer::v ={0};
float Magnetometer::mgPerDigit = 0;

void Magnetometer::init()
{
    Wire.begin();

    if ((Magnetometer::fastRegister8(HMC5883L_REG_IDENT_A) != 0x48)
    || (Magnetometer::fastRegister8(HMC5883L_REG_IDENT_B) != 0x34)
    || (Magnetometer::fastRegister8(HMC5883L_REG_IDENT_C) != 0x33))
    {
	    return;
    }

    Magnetometer::setRange(HMC5883L_RANGE_1_3GA);
    Magnetometer::setMeasurementMode(HMC5883L_CONTINOUS);
    Magnetometer::setDataRate(HMC5883L_DATARATE_30HZ);
    Magnetometer::setSamples(HMC5883L_SAMPLES_8);
    Magnetometer::setOffset(-43, -20,20);
 
    mgPerDigit = 0.92f;

    return;
}

void Magnetometer::process_magnetometer(){
//  przesun(measureDeg,n); 
  float deg = Magnetometer::readDegrees();
//  measureDeg[n-1] = deg;
//  deg = sum(measureDeg,n)/n;
  //Serial.println(deg);
  Communication::Send_Number(50,deg/2);
}

float Magnetometer::readDegrees(){
  Vector norm = Magnetometer::readNormalize();
  float Xh, Yh;  


  
  /*Xh = norm.XAxis*cos(Accelerometer::Roll) + norm.YAxis*sin(Accelerometer::Pitch)*sin(Accelerometer::Roll) + norm.ZAxis*sin(Accelerometer::Roll)*cos(Accelerometer::Pitch);
  Yh = norm.YAxis*cos(Accelerometer::Pitch) - norm.ZAxis*sin(Accelerometer::Pitch);

  Accelerometer::Pitch*=2;
  Accelerometer::Roll*=-2;
  Serial.print("Pitch ");
  Serial.print(Accelerometer::Pitch);
  Serial.print(" Roll ");
  Serial.print(Accelerometer::Roll);
  Serial.print(" Xh ");
  Serial.print(Xh);
  Serial.print(" Yh");
  Serial.print(Yh);
  Serial.print(" Mx ");
  Serial.print(norm.XAxis);
  Serial.print(" My ");
  Serial.print(norm.YAxis);
  Serial.print(" Mz ");
  Serial.println(norm.ZAxis);*/
  
  float heading = atan2(-Yh, Xh);
  //float heading = atan2(norm.YAxis, norm.XAxis);
  float declinationAngle = (4.0 + (48.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;

  // Correct for heading < 0deg and heading > 360deg
  if (heading < 0)
  {
    heading += 2 * PI;
  }

  if (heading > 2 * PI)
  {
    heading -= 2 * PI;
  }
  
  // Convert to degrees
  Serial.print(" Headnig ");
  Serial.println( heading * (float)180/(float)M_PI);
  return heading * (float)180/(float)M_PI;
}

Vector Magnetometer::readRaw(void)
{
    v.XAxis = Magnetometer::readRegister16(HMC5883L_REG_OUT_X_M) - Magnetometer::xOffset;
    v.YAxis = Magnetometer::readRegister16(HMC5883L_REG_OUT_Y_M) - Magnetometer::yOffset;
    v.ZAxis = Magnetometer::readRegister16(HMC5883L_REG_OUT_Z_M) - Magnetometer::zOffset;

    return v;
}

Vector Magnetometer::readNormalize(void)
{
    v.YAxis = -((float)Magnetometer::readRegister16(HMC5883L_REG_OUT_X_M) - Magnetometer::xOffset) * Magnetometer::mgPerDigit;
    v.XAxis = ((float)Magnetometer::readRegister16(HMC5883L_REG_OUT_Y_M) - Magnetometer::yOffset) * Magnetometer::mgPerDigit;
    v.ZAxis = (float)Magnetometer::readRegister16(HMC5883L_REG_OUT_Z_M) * Magnetometer::mgPerDigit;
  /*Serial.print(" MAG: ");
  Serial.print(" RAW X: ");
  Serial.print(v.XAxis);
  Serial.print(" RAW Y: ");
  Serial.print(v.YAxis);
  Serial.print(" RAW Z: ");
  Serial.print(-v.ZAxis);*/
    return v;
}

void Magnetometer::setOffset(int xo, int yo,int zo)
{
    Magnetometer::xOffset = xo;
    Magnetometer::yOffset = yo;
    Magnetometer::zOffset = zo;
}

void Magnetometer::setRange(hmc5883l_range_t range)
{
    switch(range)
    {
	case HMC5883L_RANGE_0_88GA:
	    Magnetometer::mgPerDigit = 0.073f;
	    break;

	case HMC5883L_RANGE_1_3GA:
	    Magnetometer::mgPerDigit = 0.92f;
	    break;

	case HMC5883L_RANGE_1_9GA:
	    Magnetometer::mgPerDigit = 1.22f;
	    break;

	case HMC5883L_RANGE_2_5GA:
	    Magnetometer::mgPerDigit = 1.52f;
	    break;

	case HMC5883L_RANGE_4GA:
	    Magnetometer::mgPerDigit = 2.27f;
	    break;

	case HMC5883L_RANGE_4_7GA:
	    Magnetometer::mgPerDigit = 2.56f;
	    break;

	case HMC5883L_RANGE_5_6GA:
	    Magnetometer::mgPerDigit = 3.03f;
	    break;

	case HMC5883L_RANGE_8_1GA:
	    Magnetometer::mgPerDigit = 4.35f;
	    break;

	default:
	    break;
    }

    Magnetometer::writeRegister8(HMC5883L_REG_CONFIG_B, range << 5);
}

void Magnetometer::setMeasurementMode(hmc5883l_mode_t mode)
{
    uint8_t value;

    value = Magnetometer::readRegister8(HMC5883L_REG_MODE);
    value &= 0b11111100;
    value |= mode;

    Magnetometer::writeRegister8(HMC5883L_REG_MODE, value);
}

void Magnetometer::setDataRate(hmc5883l_dataRate_t dataRate)
{
    uint8_t value;

    value = Magnetometer::readRegister8(HMC5883L_REG_CONFIG_A);
    value &= 0b11100011;
    value |= (dataRate << 2);

    Magnetometer::writeRegister8(HMC5883L_REG_CONFIG_A, value);
}


void Magnetometer::setSamples(hmc5883l_samples_t samples)
{
    uint8_t value;

    value = Magnetometer::readRegister8(HMC5883L_REG_CONFIG_A);
    value &= 0b10011111;
    value |= (samples << 5);

    Magnetometer::writeRegister8(HMC5883L_REG_CONFIG_A, value);
}

// Write byte to register
void Magnetometer::writeRegister8(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(HMC5883L_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

// Read byte to register
uint8_t Magnetometer::fastRegister8(uint8_t reg)
{
    uint8_t value;
    Wire.beginTransmission(HMC5883L_ADDRESS);
      Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(HMC5883L_ADDRESS, 1);
    value = Wire.read();
    Wire.endTransmission();

    return value;
}

// Read byte from register
uint8_t Magnetometer::readRegister8(uint8_t reg)
{
    uint8_t value;
    Wire.beginTransmission(HMC5883L_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.beginTransmission(HMC5883L_ADDRESS);
    Wire.requestFrom(HMC5883L_ADDRESS, 1);
    while(!Wire.available()) {};
    value = Wire.read();
    Wire.endTransmission();

    return value;
}

// Read word from register
int16_t Magnetometer::readRegister16(uint8_t reg)
{
    int16_t value;
    Wire.beginTransmission(HMC5883L_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.beginTransmission(HMC5883L_ADDRESS);
    Wire.requestFrom(HMC5883L_ADDRESS, 2);
    while(!Wire.available()) {};
    uint8_t vha = Wire.read();
    uint8_t vla = Wire.read();
    Wire.endTransmission();
    value = vha << 8 | vla;

    return value;
}

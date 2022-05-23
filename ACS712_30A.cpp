#include "Arduino.h"
#include "ACS712_30A.h"

ACS712_30A::ACS712_30A(int vinPin)
{
   _vin=vinPin;
}

float ACS712_30A::readCurrent()
{
	const float FACTOR = 66.00/1000;
	const float QOV    = 0.5 * 5.0;
	float voltage;
	float voltage_raw =   (5.0 / 1023.0)* analogRead(this->_vin);
	voltage =  voltage_raw - QOV;
	this->_outVoltage = voltage;
	this->_current = ((abs(voltage / FACTOR) - (0.0/1000))); //-22.79   abs(vol
	return this->_current; 
}

float ACS712_30A::getCurrent()
{
	float sum =0;
	for(int c=0; c < 500; c++)
	{
		sum += readCurrent();
	}
	float current = sum /500;
	return current;
}

float ACS712_30A::getVoltage()
{
	float voltage = getCurrent()*230;
	return voltage;
}

float ACS712_30A::getKiloVolt()
{
	float kiloVolt = getVoltage()/1000;
	return kiloVolt;
}

float ACS712_30A::getKWh(float i)
{
	float kwh = getKiloVolt() * i;
	return kwh;
}

float ACS712_30A::getBill(float i)
{
	float bill  = getKWh(i) *  3.5;
	return bill;
}

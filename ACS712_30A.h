#ifndef ACS712_30A_h
#define ACS712_30A_h

#include "Arduino.h"

class ACS712_30A
{
public:

    float readCurrent();
    int _vin;
    float _current,_outVoltage;

    ACS712_30A(int vinPin);	
    float getCurrent();
    float getVoltage();
    float getKiloVolt();
    float getKWh(float i);
    float getBill(float i);
	
private:

     
};

#endif

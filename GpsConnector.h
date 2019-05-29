/**
	Interface de Conexão com GPS
*/

#ifndef GpsConnector_h
#define GpsConnector_h

#include <Arduino.h>

class GpsConnector
{
public:
	virtual bool begin() = 0;
	virtual void update() = 0;
	virtual String getLatitude() = 0;
	virtual String getLongitude() = 0;
	virtual String getAltitude() = 0;
};

#endif
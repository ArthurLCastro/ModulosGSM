#ifndef Gps_h
#define Gps_h

#include <Arduino.h>
#include <GpsConnector.h>

class Gps:public GpsConnector, public ModulosGSM
{
private:
    String respGPS = "";
	String latitude = "";
	String longitude = "";
	String altitude = "";

public:
	Gps();
	bool begin();
	void update();
	String getLatitude();
	String getLongitude();
	String getAltitude();
	bool powerGPS(bool);
};

#endif
#ifndef Gps_h
#define Gps_h

#include <Arduino.h>
#include <Config.h>

class Gps{
//	private:

	public:
		Gps();

		void beginGPS();
		void updateGPS();
		String getLatitudeGPS();
		String getLongitudeGPS();
		String getAltitudeGPS();

		String latitude = "";
		String longitude = "";
		String altitude = "";
};

#endif
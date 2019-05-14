#include <Arduino.h>
#include <SoftwareSerial.h>

#ifndef ModulosGSM_h
#define ModulosGSM_h

class ModulosGSM{
	private:
		Stream *moduloGSM;
		bool comando(String, String);
	public:
		ModulosGSM();
		ModulosGSM(Stream&);
		void setupGSM(Stream&);
		String respostaGSM();
		bool testeConexaoGSM();
		String infoGSM();
		String ligarGSM(String);
		bool enviarSMS(String, String);
		String localizaGSM();
		bool httpWriteGET(String, bool);
		String httpReadGET(String, bool);
		String dadosGPS();
		bool powerGPS(bool);
		String infoGPS(unsigned int);
		String dataGPS();
		String horaGPS();
};

#endif
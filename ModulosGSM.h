#ifndef ModulosGSM_h
#define ModulosGSM_h

#include <Arduino.h>
//#include <SoftwareSerial.h>
#include <Config.h>

class ModulosGSM{
	private:
		Stream *moduloGSM;
		bool comando(String, String);
		String dadosGPS();
		String apn = "www";			// Define APN padrão como "www"
		bool estadoGPRS = false;

	public:
		ModulosGSM();
		ModulosGSM(Stream&);

		void setupGSM(Stream&);
		String respostaGSM();
		bool testeConexaoGSM();
		String infoGSM();

		String ligarGSM(String);
		bool enviarSMS(String, String);
		
		bool setGPRS();
		bool setGPRS(String);

		bool httpWriteGET(String, bool);
		String httpReadGET(String, bool);
		bool httpWritePOST(String, bool, String, String);

		bool powerGPS(bool);
		String infoGPS(unsigned int);

		String dataGPS();
		String horaGPS();

		void defineAPN(String);
};

#endif
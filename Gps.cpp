#include <Gps.h>
#include <Config.h>

Gps::Gps(){
}

bool Gps::begin(){
	powerGPS(true);
}

void Gps::update(){
  moduloGSM->flush();
  moduloGSM->print("AT+CGPSINF=2\n");
  delay(50);
  if(moduloGSM->available()>0){
    moduloGSM->flush();
    respGPS = respostaGSM();
    delay(10);
  }
}

String Gps::getLatitude(){
  return latitude;
}

String Gps::getLongitude(){
  return longitude;
}

String Gps::getAltitude(){
  return altitude;
}

bool Gps::powerGPS(bool estado){
  bool estadoComando = false;

  if(estado == true){
    estadoComando = comando("AT+CGPSPWR=1\n", "AT+CGPSPWR=1\r\nOK\r\n");
  }
  if(estado == false){
    estadoComando = comando("AT+CGPSPWR=0\n", "AT+CGPSPWR=0\r\nOK\r\n");
  }

  if(estadoComando == false){
    #ifdef DEBUG_GPS
      Serial.println("[DEBUG] ERROR comando AT+CGPSPWR");
    #endif
    return;
  }

  delay(1000);
  return estadoComando;
}
#include <ModulosGSM.h>

ModulosGSM::ModulosGSM(){
}

ModulosGSM::ModulosGSM(Stream& canalDados){
  this->moduloGSM = &canalDados;
}

void ModulosGSM::setupGSM(Stream& canalDados){
  this->moduloGSM = &canalDados;  
}

String ModulosGSM::respostaGSM(){                  // Stream é uma classe abstrata, pai de HardwareSerial e de SoftwareSerial
  String retorno = "";

  if(moduloGSM->available()>0){
    while(moduloGSM->available()>0){
      retorno += char(moduloGSM->read());
    }
    delay(5);
    moduloGSM->flush();
  }  
  return retorno;
}

bool ModulosGSM::testeConexaoGSM(){
  String retorno = "";
  bool estadoConexao = "";
  
  moduloGSM->print("AT\n");                        // Envia o comando AT de teste

  if(moduloGSM->available()>0){
    retorno = respostaGSM();
  }

  if(retorno == "AT\r\nOK\r\n"){
    estadoConexao = true;
  } else {
    estadoConexao = false;
  }

  return estadoConexao;
}

String ModulosGSM::infoGSM(){                        // Retorna Informações do GSM
  String retorno = "";

  moduloGSM->print("ATI\n");                         // Envia o comando AT solicitando informações do GSM
  retorno = respostaGSM();
  return retorno;
}

String ModulosGSM::ligarGSM(String telefone){        // Faz ligação para outro aparelho
  String retorno = "";

  moduloGSM->print("ATH\n");                         // Envia o comando AT para desligar uma ligação em andamento
  moduloGSM->print((char)26);                        // Envia o caracter referente a "Ctrl+Z" que fecha o Modo Texto no caso deste estar aberto
  moduloGSM->print("ATD" + telefone + ";\n");        // Executa o comando AT para efetuar ligação
  
  if(moduloGSM->available()>0){
    retorno = respostaGSM(); 
  }

  return retorno;
}

bool ModulosGSM::TestgetGSM(String urlDados){
  bool estadoEnvio = false;

  estadoEnvio = comando("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\n", "AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\nOK\r\n");
  delay(50);
  if(estadoEnvio == false){
    return estadoEnvio;
    break;
  }
  estadoEnvio = comando("AT+SAPBR=3,1,\"APN\",\"www\"\n", ""); // Adicionar respEsperada
  delay(50);
  if(estadoEnvio == false){
    return estadoEnvio;
    break;
  }
  estadoEnvio = comando("AT+SAPBR=1,1\n", ""); // Adicionar respEsperada
  delay(50);
  if(estadoEnvio == false){
    return estadoEnvio;
    break;
  }
  estadoEnvio = comando("AT+SAPBR=2,1\n", ""); // Adicionar respEsperada
  delay(50);
  if(estadoEnvio == false){
    return estadoEnvio;
    break;
  }
  estadoEnvio = comando("AT+HTTPINIT\n", ""); // Adicionar respEsperada
  delay(50);
  if(estadoEnvio == false){
    return estadoEnvio;
    break;
  }
  estadoEnvio = comando("AT+HTTPSSL=1\n", ""); // Adicionar respEsperada
  delay(50);
  if(estadoEnvio == false){
    return estadoEnvio;
    break;
  }
  estadoEnvio = comando("AT+HTTPPARA=\"CID\",1\n", ""); // Adicionar respEsperada
  delay(50);
  if(estadoEnvio == false){
    return estadoEnvio;
    break;
  }
  estadoEnvio = comando("AT+HTTPPARA=\"URL\",\"" + urlDados + "\n", ""); // Adicionar respEsperada
  delay(50);
  if(estadoEnvio == false){
    return estadoEnvio;
    break;
  }
  estadoEnvio = comando("AT+HTTPACTION=0\n", ""); // Adicionar respEsperada
  delay(50);
  if(estadoEnvio == false){
    return estadoEnvio;
    break;
  }

  return estadoEnvio; 

/*
//Avaliar o RISCO do módulo travar no While caso não haja conexão com o módulo
  while (estadoEnvio != true){
    moduloGSM->print("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\n");
    if(moduloGSM->available()>0){
      if(respostaGSM() == "AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\nOK\r\n"){
        estadoEnvio = true;
      } else {
        estadoEnvio = false;
      }
    }
    delay(50);
  }
*/
}

bool ModulosGSM::comando(String comandoAT, String respEsperada){
  static unsigned int tentativas = 5;
  bool comandOk = false, retorno = false;
  unsigned int i=1;

  for(i=1; i<=tentativas; i++){
    if (comandOk == false){
      moduloGSM->print(comandoAT);
      if(moduloGSM->available()>0){
        if(respostaGSM() == respEsperada){
          comandOk = true;
          retorno = true;
        } else {
          comandOk = false;
        }
      }
    } else {
      retorno = true;
      break;
    }
    delay(100);
  }
}

bool ModulosGSM::getGSM(String urlDados){
  bool estadoConexao = false;

  int tempo = 5000;

  while (estadoConexao != true){
    moduloGSM->print("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
    delay(tempo);
    if(moduloGSM->available()>0){
      if(respostaGSM() == "AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\nOK\r\n"){
        estadoConexao = true;
      } else {
        estadoConexao = false;
      }
    }
  }
  moduloGSM->print("AT+SAPBR=3,1,\"APN\",\"www\"\n");
  delay(tempo);
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  moduloGSM->print("AT+SAPBR=1,1\n");
  delay(tempo);
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  moduloGSM->print("AT+SAPBR=2,1\n");
  delay(tempo);
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  moduloGSM->print("AT+HTTPINIT\n");
  delay(tempo);
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  moduloGSM->print("AT+HTTPSSL=1\n");
  delay(tempo);
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  moduloGSM->print("AT+HTTPPARA=\"CID\",1\n");
  delay(tempo);
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  moduloGSM->print("AT+HTTPPARA=\"URL\",\"" + urlDados + "\n");
  delay(tempo);
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  moduloGSM->print("AT+HTTPACTION=0\n");
  delay(tempo);
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
//  moduloGSM->print("AT+HTTPREAD\n");
//  delay(5);

  estadoConexao = true;
  return estadoConexao;
}

/*

bool ModulosGSM::getGSM(){
  //String retorno = "";
  bool estadoConexao = "";
  int tempo = 1000;

  moduloGSM->print("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  delay(tempo);
  moduloGSM->print("AT+SAPBR=3,1,\"APN\",\"www\"\n");
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  delay(tempo);
  moduloGSM->print("AT+SAPBR=1,1\n");
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  delay(tempo);
  moduloGSM->print("AT+SAPBR=2,1\n");
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  delay(tempo);
  moduloGSM->print("AT+HTTPINIT\n");
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  delay(tempo);
  moduloGSM->print("AT+HTTPSSL=1\n");
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  delay(tempo);
  moduloGSM->print("AT+HTTPPARA=\"CID\",1\n");
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  delay(tempo);
  moduloGSM->print("AT+HTTPPARA=\"URL\",\"https://castroarthurelectronics.000webhostapp.com/get/add.php?temp=28&umid=25&lum=70\"\n");
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  delay(tempo);
  moduloGSM->print("AT+HTTPACTION=0\n");
  if(moduloGSM->available()>0){
    Serial.print(respostaGSM());
  }
  delay(tempo);
//  moduloGSM->print("AT+HTTPREAD\n");
//  delay(5);

  estadoConexao = true;
  return estadoConexao;
}
*/
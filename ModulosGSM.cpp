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

bool ModulosGSM::httpWriteGET(String urlDados){
  bool estadoEnvio = false;

  estadoEnvio = comando("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\n", "AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\nOK\r\n");
  delay(50);
  if(estadoEnvio == false){
Serial.println("[DEBUG] ERROR comando AT 1");
    return;
  }
  estadoEnvio = comando("AT+SAPBR=3,1,\"APN\",\"www\"\n", "AT+SAPBR=3,1,\"APN\",\"www\"\r\nOK\r\n");
  delay(50);
  if(estadoEnvio == false){
Serial.println("[DEBUG] ERROR comando AT 2");
    return;
  }
  estadoEnvio = comando("AT+SAPBR=1,1\n", "Qualquer");
  delay(50);

  estadoEnvio = comando("AT+SAPBR=2,1\n", "AT+SAPBR=2,1\r\nERROR\r\n");
  delay(50);
  if(estadoEnvio == true){
Serial.println("[DEBUG] ERROR comando AT 4");
    return;
  }
  estadoEnvio = comando("AT+HTTPINIT\n", "Qualquer");
  delay(50);

  estadoEnvio = comando("AT+HTTPSSL=1\n", "AT+HTTPSSL=1\r\nOK\r\n");
  delay(50);
  if(estadoEnvio == false){
Serial.println("[DEBUG] ERROR comando AT 6");
    return;
  }
  estadoEnvio = comando("AT+HTTPPARA=\"CID\",1\n", "AT+HTTPPARA=\"CID\",1\r\nOK\r\n");
  delay(50);
  if(estadoEnvio == false){
Serial.println("[DEBUG] ERROR comando AT 7");
    return;
  }
  estadoEnvio = comando("AT+HTTPPARA=\"URL\",\"" + urlDados + "\"\n", "Qualquer");
  delay(50);
  if(estadoEnvio == false){
Serial.println("[DEBUG] ERROR comando AT 8");
    return;
  }
  estadoEnvio = comando("AT+HTTPACTION=0\n", "Qualquer");
  delay(50);

  return estadoEnvio;         //Comando só será executado se o estadoEnvio for true

}

bool ModulosGSM::comando(String comandoAT, String respEsperada){
  static unsigned int tentativas = 5;
  bool comandOk = false, retorno = false;
  unsigned int i=1;

Serial.print("[DEBUG] comandoAT: ");
Serial.println(comandoAT);

  for(i=1; i<=tentativas; i++){
    if (comandOk == false){
      moduloGSM->print(comandoAT);
      if(moduloGSM->available()>0){

        if(respEsperada == "Qualquer"){
          comandOk = true;
          return retorno = true;
        } else {
          if(respostaGSM() == respEsperada){
            comandOk = true;
            return retorno = true;
          } else {
            comandOk = false;
          }
        }
  
      }
    } else {
      return retorno = true;
    }
    delay(100);
  }

  return retorno;
}

String ModulosGSM::httpReadGET(String urlDados){
  bool estadoEnvio;
  String retorno = "", pagina = "";

  estadoEnvio = httpWriteGET(urlDados);

  if(estadoEnvio == true){
    moduloGSM->print("AT+HTTPREAD\n");
      if(moduloGSM->available()>0){
        pagina = respostaGSM();
        if(pagina != "AT+HTTPREAD\r\nERROR\r\n"){
          retorno = pagina;
        } else {
          retorno = "[DEBUG] Erro na leitura da Pagina";
        }
      }
    delay(50);
  } else {
    retorno = "[DEBUG] Erro ao executar GET";
  }
  return retorno;
}
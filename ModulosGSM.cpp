#include <ModulosGSM.h>

// Descomentando a linha abaixo será possível vizualizar o DEBUG pela Serial
//#define DEBUG
//#define DEBUG_GPS
#define RespComand

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
/*
  if(testeConexaoGSM() == false){
    return "[DEBUG_ligarGSM()] Falha na conexão do Módulo GSM";
  }
*/
  moduloGSM->print("ATH\n");                         // Envia o comando AT para desligar uma ligação em andamento
  moduloGSM->print((char)26);                        // Envia o caracter referente a "Ctrl+Z" que fecha o Modo Texto no caso deste estar aberto
  moduloGSM->print("ATD" + telefone + ";\n");        // Executa o comando AT para efetuar ligação
  
  if(moduloGSM->available()>0){
    retorno = respostaGSM();
  }

  return retorno;
}

bool ModulosGSM::enviarSMS(String telefone, String mensagem){               // Envia uma mensagem de texto SMS para outro aparelho
  bool conexaoSMS = false;
  String respEsperada = "", resp = "";
  String tamanhoMsg = "";

  conexaoSMS = comando("AT+CMGS=?\n", "AT+CMGS=?\r\nOK\r\n");     // Envia o comando de Teste
  delay(50);
  if(conexaoSMS == false){
    return false;
  }

  conexaoSMS = comando("AT+CMGF=1\n", "AT+CMGF=1\r\nOK\r\n");     // Envia o comando de Teste
  delay(50);
  if(conexaoSMS == false){
    return false;
  }

  delay(1000);

  tamanhoMsg = String(mensagem.length());
  respEsperada = "+CMGS: " + tamanhoMsg + "\r\n\r\nOK\r\n";

  moduloGSM->print("AT+CMGS=\"" + telefone + "\"\n");           // Inicia Modo Texto para envio de mensagem
  moduloGSM->print(mensagem + "\n");                            // Escreve a Mensagem
  moduloGSM->print((char)26);                                   // Envia o caracter referente a "Ctrl+Z" que fecha o Modo Texto no caso deste estar aberto

  delay(10000);

  if(moduloGSM->available()>0){
    resp = respostaGSM();
    delay(50);
    if(resp == respEsperada){
      conexaoSMS = true;
    } else {
      conexaoSMS = false;
    }
  }

  #ifdef DEBUG
    Serial.println("[SMS] respEsperada: ");
    Serial.println(respEsperada);
    Serial.println("[SMS] respostaGSM: ");
    Serial.println(resp);
  #endif

  return conexaoSMS;
}

bool ModulosGSM::comando(String comandoAT, String respEsperada){
  static unsigned int tentativas = 10;
  bool comandOk = false, retorno = false;
  unsigned int i=1;
  String respRecebida = "";

  #ifdef DEBUG
    Serial.print("[DEBUG] comandoAT: ");
    Serial.println(comandoAT);
  #endif

  for(i=1; i<=tentativas; i++){
    if (comandOk == false){
      moduloGSM->print(comandoAT);
      if(moduloGSM->available()>0){
        if(respEsperada == "Qualquer"){
          comandOk = true;
          return retorno = true;
        } else {
          respRecebida = respostaGSM();
          if(respRecebida == respEsperada){
            comandOk = true;
            #ifdef RespComand
                Serial.print("[ComandoAT] Tentativas ate OK: ");
                Serial.println(i);              
            #endif
            return retorno = true;
          } else {
            comandOk = false;
            #ifdef RespComand
              if(i == 5){
                Serial.println("[ERROR] comandoAT: ");
                Serial.println(comandoAT);
                Serial.println("[ERROR] respRecebida: ");
                Serial.println(respRecebida);
              }
            #endif
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

bool ModulosGSM::setGPRS(){
  bool estadoGPRS = false;

  /*
    Adicionar condição de execução:
    Se o GPRS já estiver ativo não executar o resto da função setGPRS()
  */

  estadoGPRS = comando("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\n", "AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\nOK\r\n");
  delay(50);
  if(estadoGPRS == false){
    #ifdef DEBUG
      Serial.println("[DEBUG] ERROR comando AT+SAPBR Contype");
    #endif
    return;
  }
  estadoGPRS = comando("AT+SAPBR=3,1,\"APN\",\"www\"\n", "AT+SAPBR=3,1,\"APN\",\"www\"\r\nOK\r\n");
  delay(50);
  if(estadoGPRS == false){
    #ifdef DEBUG
      Serial.println("[DEBUG] ERROR comando AT+SAPBR APN");
    #endif
    return;
  }
  estadoGPRS = comando("AT+SAPBR=1,1\n", "Qualquer");
  delay(50);

  estadoGPRS = comando("AT+SAPBR=2,1\n", "AT+SAPBR=2,1\r\nERROR\r\n");
  delay(50);
  if(estadoGPRS == true){
    #ifdef DEBUG
      Serial.println("[DEBUG] ERROR comando AT+SAPBR=2,1");
    #endif
    return;
  } else {
    estadoGPRS = true;
  }

  #ifdef DEBUG
    Serial.print("estadoGPRS: ");
    Serial.println(estadoGPRS);
  #endif

  return estadoGPRS;         //Comando só será executado se o estadoGPRS for true
}

bool ModulosGSM::httpWriteGET(String urlDados, bool https){
  bool estadoEnvio = false;

  estadoEnvio = setGPRS();
  delay(50);
  if(estadoEnvio == false){
    #ifdef DEBUG
      Serial.println("[DEBUG] ERROR GPRS Inativo");
    #endif
    return estadoEnvio;
  }
  estadoEnvio = comando("AT+HTTPINIT\n", "Qualquer");
  delay(50);

  if(https == 1){
    estadoEnvio = comando("AT+HTTPSSL=1\n", "AT+HTTPSSL=1\r\nOK\r\n");
    delay(50);
    if(estadoEnvio == false){
      #ifdef DEBUG
        Serial.println("[DEBUG] ERROR comando AT+HTTPSSL");
      #endif
      return estadoEnvio;
    }
  }
  estadoEnvio = comando("AT+HTTPPARA=\"CID\",1\n", "AT+HTTPPARA=\"CID\",1\r\nOK\r\n");
  delay(50);
  if(estadoEnvio == false){
    #ifdef DEBUG
      Serial.println("[DEBUG] ERROR comando AT+HTTPPARA CID");
    #endif
    return estadoEnvio;
  }
  estadoEnvio = comando("AT+HTTPPARA=\"URL\",\"" + urlDados + "\"\n", "Qualquer");
  delay(50);
  if(estadoEnvio == false){
    #ifdef DEBUG
      Serial.println("[DEBUG] ERROR comando AT+HTTPPARA URL");
    #endif
    return estadoEnvio;
  }
  estadoEnvio = comando("AT+HTTPACTION=0\n", "Qualquer");
  delay(50);

  #ifdef DEBUG
    Serial.print("estadoEnvio: ");
    Serial.println(estadoEnvio);
  #endif

  return estadoEnvio;         //Comando só será executado se o estadoEnvio for true
}

String ModulosGSM::httpReadGET(String url, bool https){
  bool estadoEnvio;
  String retorno = "", pagina = "";

  estadoEnvio = httpWriteGET(url, https);

  if(estadoEnvio == true){
    moduloGSM->print("AT+HTTPREAD\n");
    #ifdef DEBUG
      Serial.println("READ executado");
    #endif
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

bool ModulosGSM::httpWritePOST(String url, bool https, String contentType, String pacote){
  bool estadoEnvio = false;
  static unsigned int tempoEscritaVariaveis = 5000;

  estadoEnvio = setGPRS();
  delay(50);
  if(estadoEnvio == false){
    #ifdef DEBUG
      Serial.println("[DEBUG] ERROR GPRS Inativo");
    #endif
    return false;
  }
  estadoEnvio = comando("AT+HTTPINIT\n", "Qualquer");
  delay(50);

  if(https == 1){
    estadoEnvio = comando("AT+HTTPSSL=1\n", "AT+HTTPSSL=1\r\nOK\r\n");
    delay(50);
    if(estadoEnvio == false){
      #ifdef DEBUG
        Serial.println("[DEBUG] ERROR comando AT+HTTPSSL");
      #endif
      return false;
    }
  }
  estadoEnvio = comando("AT+HTTPPARA=\"CID\",1\n", "AT+HTTPPARA=\"CID\",1\r\nOK\r\n");
  delay(50);
  if(estadoEnvio == false){
    #ifdef DEBUG
      Serial.println("[DEBUG] ERROR comando AT+HTTPPARA CID");
    #endif
    return false;
  }
  estadoEnvio = comando("AT+HTTPPARA=\"URL\",\"" + url + "\"\n", "Qualquer");
  delay(50);
  // Verificar se o próximo if é necessário:
  if(estadoEnvio == false){
    #ifdef DEBUG
      Serial.println("[DEBUG] ERROR comando AT+HTTPPARA URL");
    #endif
    return false;
  }

  estadoEnvio = comando("AT+HTTPPARA=\"CONTENT\",\"" + contentType + "\"\n", "Qualquer");
  delay(50);
  if(estadoEnvio == false){
    #ifdef DEBUG
      Serial.println("[DEBUG] ERROR comando AT+HTTPPARA CONTENT");
    #endif
    return false;
  }
  // Verificar se o tamanho 192 é adequado e utilizar milis ao invés de delay
  estadoEnvio = comando("AT+HTTPDATA=192," + String(tempoEscritaVariaveis) + "\n", "Qualquer");
  delay(1000);
  moduloGSM->print(pacote);
  delay(tempoEscritaVariaveis);
  delay(3000);

  estadoEnvio = comando("AT+HTTPACTION=1\n", "Qualquer");
  delay(50);

  #ifdef DEBUG
    Serial.print("estadoEnvio: ");
    Serial.println(estadoEnvio);
  #endif

  return estadoEnvio;         //Comando só será executado se o estadoEnvio for true
}

bool ModulosGSM::powerGPS(bool estado){
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

String ModulosGSM::dadosGPS(){
  String resp = "";

  moduloGSM->flush();
  moduloGSM->print("AT+CGPSINF=2\n");
  delay(50);
  if(moduloGSM->available()>0){
    moduloGSM->flush();
    resp = respostaGSM();
    delay(10);
  }

  return resp;
}

String ModulosGSM::infoGPS(unsigned int informacao){
  String respostaGSM = "",
         restoInfo1 = "",
         restoInfo2 = "",
         restoInfo3 = "",
         restoInfo4 = "",
         restoInfo5 = "",
         restoInfo6 = "",
         restoInfo7 = "",
         restoInfo8 = "",
         restoInfo9 = "",
         restoInfo10 = "",
         restoInfo11 = "",
         restoInfo12 = "",
         restoInfo13 = "";

  unsigned int indiceVirg1 = 0,
               indiceVirg2 = 0,
               indiceVirg3 = 0,
               indiceVirg4 = 0,
               indiceVirg5 = 0,
               indiceVirg6 = 0,
               indiceVirg7 = 0,
               indiceVirg8 = 0,
               indiceVirg9 = 0,
               indiceVirg10 = 0,
               indiceVirg11 = 0,
               indiceVirg12 = 0,
               indiceVirg13 = 0;

  String hora = "",
         lat = "",
         indicadorNS = "",
         lon = "",
         indicadorEW = "",
         posFixInd = "",
         satelites = "",
         HDOP = "",
         altitude = "",
         units = "",
         geoidSepar = "",
         units2 = "";

  respostaGSM = dadosGPS();
  #ifdef DEBUG_GPS
    Serial.print("[DEBUG_infoGPS] respostaGSM: ");
    Serial.println(respostaGSM);
  #endif

  indiceVirg1 = respostaGSM.indexOf(',');
  restoInfo1 = respostaGSM.substring(indiceVirg1 + 1);

  indiceVirg2 = restoInfo1.indexOf(',');
  restoInfo2 = restoInfo1.substring(indiceVirg2 + 1);

  indiceVirg3 = restoInfo2.indexOf(',');
  restoInfo3 = restoInfo2.substring(indiceVirg3 + 1);

  indiceVirg4 = restoInfo3.indexOf(',');
  restoInfo4 = restoInfo3.substring(indiceVirg4 + 1);

  indiceVirg5 = restoInfo4.indexOf(',');
  restoInfo5 = restoInfo4.substring(indiceVirg5 + 1);

  indiceVirg6 = restoInfo5.indexOf(',');
  restoInfo6 = restoInfo5.substring(indiceVirg6 + 1);

  indiceVirg7 = restoInfo6.indexOf(',');
  restoInfo7 = restoInfo6.substring(indiceVirg7 + 1);

  indiceVirg8 = restoInfo7.indexOf(',');
  restoInfo8 = restoInfo7.substring(indiceVirg8 + 1);

  indiceVirg9 = restoInfo8.indexOf(',');
  restoInfo9 = restoInfo8.substring(indiceVirg9 + 1);

  indiceVirg10 = restoInfo9.indexOf(',');
  restoInfo10 = restoInfo9.substring(indiceVirg10 + 1);

  indiceVirg11 = restoInfo10.indexOf(',');
  restoInfo11 = restoInfo10.substring(indiceVirg11 + 1);

  indiceVirg12 = restoInfo11.indexOf(',');
  restoInfo12 = restoInfo11.substring(indiceVirg12 + 1);

  indiceVirg13 = restoInfo12.indexOf(',');
  restoInfo13 = restoInfo12.substring(indiceVirg13 + 1);

  // ...
  indiceVirg2 = indiceVirg2 + indiceVirg1;
  indiceVirg3 = indiceVirg3 + indiceVirg2;
  indiceVirg4 = indiceVirg4 + indiceVirg3;
  indiceVirg5 = indiceVirg5 + indiceVirg4;
  indiceVirg6 = indiceVirg6 + indiceVirg5;
  indiceVirg7 = indiceVirg7 + indiceVirg6;
  indiceVirg8 = indiceVirg8 + indiceVirg7;
  indiceVirg9 = indiceVirg9 + indiceVirg8;
  indiceVirg10 = indiceVirg10 + indiceVirg9;
  indiceVirg11 = indiceVirg11 + indiceVirg10;
  indiceVirg12 = indiceVirg12 + indiceVirg11;
  indiceVirg13 = indiceVirg13 + indiceVirg12;

  hora = respostaGSM.substring(indiceVirg1 + 1, indiceVirg2 + 1);
  lat = respostaGSM.substring(indiceVirg2 + 2, indiceVirg3 + 2);
  indicadorNS = respostaGSM.substring(indiceVirg3 + 3, indiceVirg4 + 3);
  lon = respostaGSM.substring(indiceVirg4 + 4, indiceVirg5 + 4);
  indicadorEW = respostaGSM.substring(indiceVirg5 + 5, indiceVirg6 + 5);
  posFixInd = respostaGSM.substring(indiceVirg6 + 6, indiceVirg7 + 6);
  satelites = respostaGSM.substring(indiceVirg7 + 7, indiceVirg8 + 7);
  HDOP = respostaGSM.substring(indiceVirg8 + 8, indiceVirg9 + 8);
  altitude = respostaGSM.substring(indiceVirg9 + 9, indiceVirg10 + 9);
  units = respostaGSM.substring(indiceVirg10 + 10, indiceVirg11 + 10);
  geoidSepar = respostaGSM.substring(indiceVirg11 + 11, indiceVirg12 + 11);
  units2 = respostaGSM.substring(indiceVirg12 + 12, indiceVirg13 + 12);

  #ifdef DEBUG_GPS
    Serial.print("[DEBUG_infoGPS] hora: ");
    Serial.println(hora);
    Serial.print("[DEBUG_infoGPS] lat: ");
    Serial.println(lat);
    Serial.print("[DEBUG_infoGPS] indicadorNS: ");
    Serial.println(indicadorNS);
    Serial.print("[DEBUG_infoGPS] lon: ");
    Serial.println(lon);
    Serial.print("[DEBUG_infoGPS] indicadorEW: ");
    Serial.println(indicadorEW);
    Serial.print("[DEBUG_infoGPS] posFixInd: ");
    Serial.println(posFixInd);
    Serial.print("[DEBUG_infoGPS] satelites: ");
    Serial.println(satelites);
    Serial.print("[DEBUG_infoGPS] HDOP: ");
    Serial.println(HDOP);
    Serial.print("[DEBUG_infoGPS] altitude: ");
    Serial.println(altitude);
    Serial.print("[DEBUG_infoGPS] units: ");
    Serial.println(units);
    Serial.print("[DEBUG_infoGPS] geoidSepar: ");
    Serial.println(geoidSepar);
    Serial.print("[DEBUG_infoGPS] units2: ");
    Serial.println(units2);
  #endif

  switch(informacao){
    case 0:
    default:
      return respostaGSM;
    case 1:
      return hora;
    case 2:
      return lat;
    case 3:
      return indicadorNS;
    case 4:
      return lon;
    case 5:
      return indicadorEW;
    case 6:
      return posFixInd;
    case 7:
      return satelites;
    case 8:
      return HDOP;
    case 9:
      return altitude;
    case 10:
      return units;
    case 11:
      return geoidSepar;
    case 12:
      return units2;
  }
}
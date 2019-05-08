#include <ModulosGSM.h>

// Descomentando a linha abaixo será possível vizualizar o DEBUG pela Serial
//#define DEBUG

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

bool ModulosGSM::enviarSMS(String telefone, String mensagem){               // Envia uma mensagem de texto SMS para outro aparelho
  bool conexaoSMS = false;

  conexaoSMS = comando("AT+CMGS=?\n", "AT+CMGS=?\r\nOK\r\n");     // Envia o comando de Teste
  delay(50);
  if(conexaoSMS == false){
    return;
  }

  conexaoSMS = comando("AT+CMGF=1\n", "AT+CMGF=1\r\nOK\r\n");     // Envia o comando de Teste
  delay(50);
  if(conexaoSMS == false){
    return;
  }

  moduloGSM->print("AT+CMGS=\"" + telefone + "\"\n");           // Inicia Modo Texto para envio de mensagem
  moduloGSM->print(mensagem + "\n");                            // Escreve a Mensagem
  moduloGSM->print((char)26);                                   // Envia o caracter referente a "Ctrl+Z" que fecha o Modo Texto no caso deste estar aberto

/*
  if(moduloGSM->available()>0){
    String resp = respostaGSM();
    if (resp CONTEM "OK"){    // Criar função para comparar o conteúdo de uma frase com uma palavra específica
      conexaoSMS = true;
    } else {
      conexaoSMS = false;  
    }
  }
*/

  return conexaoSMS;        // Ainda não muito confiável
}

String ModulosGSM::localizaGSM(){
/*
AT+CGPSPWR=1
AT+CGPSSTATUS?
AT+CGPSINF=0
AT+CGPSOUT=8
AT+CGPSRST=0
AT+CGPSRST=1
AT+CGPSPWR=0

AT+CGPSPWR=1
AT+CGPSRST=1
AT+CGPSSTATUS?
AT+CGPSINF=0
AT+CGPSINF=32
AT+CGPSSTATUS?
AT+CGPSOUT=32
AT+CGPSOUT=0
*/
}


bool ModulosGSM::httpWriteGET(String urlDados, bool https){
  bool estadoEnvio = false;

  estadoEnvio = comando("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\n", "AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\nOK\r\n");
  delay(50);
  if(estadoEnvio == false){
    #ifdef DEBUG
      Serial.println("[DEBUG] ERROR comando AT 1");
    #endif
    return;
  }
  estadoEnvio = comando("AT+SAPBR=3,1,\"APN\",\"www\"\n", "AT+SAPBR=3,1,\"APN\",\"www\"\r\nOK\r\n");
  delay(50);
  if(estadoEnvio == false){
    #ifdef DEBUG
      Serial.println("[DEBUG] ERROR comando AT 2");
    #endif
    return;
  }
  estadoEnvio = comando("AT+SAPBR=1,1\n", "Qualquer");
  delay(50);

  estadoEnvio = comando("AT+SAPBR=2,1\n", "AT+SAPBR=2,1\r\nERROR\r\n");
  delay(50);
  if(estadoEnvio == true){
    #ifdef DEBUG
      Serial.println("[DEBUG] ERROR comando AT 4");
    #endif
    return;
  }
  estadoEnvio = comando("AT+HTTPINIT\n", "Qualquer");
  delay(50);

  if(https == 1){
    estadoEnvio = comando("AT+HTTPSSL=1\n", "AT+HTTPSSL=1\r\nOK\r\n");
    delay(50);
    if(estadoEnvio == false){
      #ifdef DEBUG
        Serial.println("[DEBUG] ERROR comando AT 6");
      #endif
      return;
    }
  }
  estadoEnvio = comando("AT+HTTPPARA=\"CID\",1\n", "AT+HTTPPARA=\"CID\",1\r\nOK\r\n");
  delay(50);
  if(estadoEnvio == false){
    #ifdef DEBUG
      Serial.println("[DEBUG] ERROR comando AT 7");
    #endif
    return;
  }
  estadoEnvio = comando("AT+HTTPPARA=\"URL\",\"" + urlDados + "\"\n", "Qualquer");
  delay(50);
  if(estadoEnvio == false){
    #ifdef DEBUG
      Serial.println("[DEBUG] ERROR comando AT 8");
    #endif
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
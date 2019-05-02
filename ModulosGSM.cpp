#include <ModulosGSM.h>

ModulosGSM::ModulosGSM(){
}

ModulosGSM::ModulosGSM(Stream& canalDados){
  this->moduloGSM = &canalDados;
}

void ModulosGSM::setupGSM(Stream& canalDados){
  this->moduloGSM = &canalDados;  
}

String ModulosGSM::respostaGSM(){        // Stream é uma classe abstrata, pai de HardwareSerial e de SoftwareSerial
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

String ModulosGSM::infoGSM(){                      // Retorna Informações do GSM
  String retorno = "";

  moduloGSM->print("ATI\n");                        // Envia o comando AT solicitando informações do GSM
  retorno = respostaGSM();
  return retorno;
}

String ModulosGSM::ligarGSM(String telefone){       // Faz ligação para outro aparelho
  String retorno = "";

  moduloGSM->print("ATH\n");                         // Envia o comando AT para desligar uma ligação em andamento
  moduloGSM->print((char)26);                        // Envia o caracter referente a "Ctrl+Z" que fecha o Modo Texto no caso deste estar aberto
  moduloGSM->print("ATD" + telefone + ";\n");        // Executa o comando AT para efetuar ligação
  
  if(moduloGSM->available()>0){
    retorno = respostaGSM(); 
  }

  return retorno;
}
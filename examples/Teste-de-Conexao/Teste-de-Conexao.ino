// Teste de Conexão com o Módulo
// Arthur L. Castro
// Abril de 2019

// INCLUSÃO DE BIBLIOTECAS:
#include <ModulosGSM.h>

// DEFINIÇÕES:
#define RX_GSM1 8         // ligar o Pino 8 do Arduino no TX do Módulo "SIM808 EVB-V3.2"
#define TX_GSM1 7         // ligar o Pino 7 do Arduino no RX do Módulo "SIM808 EVB-V3.2"
#define intervalo 500

// CRIAÇÃO DE OBJETOS - Para emular as comunicações Seriais nos pinos digitais definidos acima:
ModulosGSM meuGSM1;
SoftwareSerial serialGSM1(RX_GSM1, TX_GSM1);

// DECLARAÇÕES DE VARIÁVEIS GLOBAIS:
bool teste;

void setup(){
  Serial.begin(9600);                       // Inicia a comunicação Serial a uma taxa de transmissão de 9600
  serialGSM1.begin(9600);                   // Inicia a comunicação Serial Emulada a uma taxa de transmissão de 9600
  meuGSM1.setupGSM(serialGSM1);
}

void loop(){
  teste = meuGSM1.testeConexaoGSM();        // Retorna um valor booleano, 1 para conexao ok ou 0 para módulo desconectado
  Serial.println(teste);

  if(meuGSM1.testeConexaoGSM() == 1){
    Serial.println("Conexao Estabelecida");
  } else {
    Serial.println("Modulo Desconectado");  
  }

  delay(intervalo);
}
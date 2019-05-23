// Efetuar Chamada de Voz
// Arthur L. Castro
// Abril de 2019

// INCLUSÃO DE BIBLIOTECAS:
#include <ModulosGSM.h>

// DEFINIÇÕES:
#define RX_GSM1 8         // ligar o Pino 8 do Arduino no TX do Módulo "SIM808 EVB-V3.2"
#define TX_GSM1 7         // ligar o Pino 7 do Arduino no RX do Módulo "SIM808 EVB-V3.2"
#define intervalo 20000   // Intervalo entre as chamadas

// CRIAÇÃO DE OBJETOS - Para emular as comunicações Seriais nos pinos digitais definidos acima:
ModulosGSM meuGSM1;

// DECLARAÇÕES DE VARIÁVEIS GLOBAIS:
SoftwareSerial serialGSM1(RX_GSM1, TX_GSM1);
String numero = "Insira_aqui_o_número_para_o_qual_a_ligação_será_feita";

void setup(){
  Serial.begin(9600);                       // Inicia a comunicação Serial a uma taxa de transmissão de 9600
  serialGSM1.begin(9600);                   // Inicia a comunicação Serial Emulada a uma taxa de transmissão de 9600
  meuGSM1.setupGSM(serialGSM1);
}

void loop(){
  meuGSM1.ligarGSM(numero);                 // Retorna uma String com "AT OK" por enquanto
  Serial.print("Ligando para ");
  Serial.println(numero);
  delay(intervalo);
}
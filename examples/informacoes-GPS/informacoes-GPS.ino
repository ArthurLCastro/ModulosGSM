// Captura de Localização via GPS
// Arthur L. Castro
// Maio de 2019

// INCLUSÃO DE BIBLIOTECAS:
#include <ModulosGSM.h>

// DEFINIÇÕES:
#define RX_GSM1 8         // ligar o Pino 8 do Arduino no TX do Módulo "SIM808 EVB-V3.2"
#define TX_GSM1 7         // ligar o Pino 7 do Arduino no RX do Módulo "SIM808 EVB-V3.2"
#define estabilizarGPS 5000
#define intervalo 5000    // Intervalo entre atualizações da localização

// CRIAÇÃO DE OBJETOS - Para emular as comunicações Seriais nos pinos digitais definidos acima:
ModulosGSM meuGSM1;
SoftwareSerial serialGSM1(RX_GSM1, TX_GSM1);

// DECLARAÇÕES DE VARIÁVEIS GLOBAIS:
String info = "";

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

void setup(){
  Serial.begin(9600);                       // Inicia a comunicação Serial a uma taxa de transmissão de 9600
  serialGSM1.begin(9600);                   // Inicia a comunicação Serial Emulada a uma taxa de transmissão de 9600
  meuGSM1.setupGSM(serialGSM1);

  meuGSM1.powerGPS(true);               // Habilita a alimentação do GPS
//  Serial.println("[DEBUG] Aguardando estabilizacao do GPS...");
//  delay(estabilizarGPS);                // Tempo para estabilizar a conexão do GPS
}

void loop(){
//  info = meuGSM1.infoGPS(0);

  hora = meuGSM1.infoGPS(1);
  lat = meuGSM1.infoGPS(2);
  indicadorNS = meuGSM1.infoGPS(3);
  lon = meuGSM1.infoGPS(4);
  indicadorEW = meuGSM1.infoGPS(5);
  posFixInd = meuGSM1.infoGPS(6);
  satelites = meuGSM1.infoGPS(7);
  HDOP = meuGSM1.infoGPS(8);
  altitude = meuGSM1.infoGPS(9);
  units = meuGSM1.infoGPS(10);
  geoidSepar = meuGSM1.infoGPS(11);
  units2 = meuGSM1.infoGPS(12);

//  meuGSM1.powerGPS(false);               // Desabilita a alimentação do GPS

  Serial.println("-------------------------------------");
  Serial.println("|     Localizacao do Modulo GSM     |");
  Serial.println("-------------------------------------");
//  Serial.print("\t> Informacoes: ");
//  Serial.println(info);
  Serial.print("\t> hora: ");
  Serial.println(hora);
  Serial.print("\t> lat: ");
  Serial.println(lat);
  Serial.print("\t> indicadorNS: ");
  Serial.println(indicadorNS);
  Serial.print("\t> lon: ");
  Serial.println(lon);
  Serial.print("\t> indicadorEW: ");
  Serial.println(indicadorEW);
  Serial.print("\t> posFixInd: ");
  Serial.println(posFixInd);
  Serial.print("\t> satelites: ");
  Serial.println(satelites);
  Serial.print("\t> HDOP: ");
  Serial.println(HDOP);
  Serial.print("\t> altitude: ");
  Serial.println(altitude);
  Serial.print("\t> units: ");
  Serial.println(units);
  Serial.print("\t> geoidSepar: ");
  Serial.println(geoidSepar);
  Serial.print("\t> units2: ");
  Serial.println(units2);
  Serial.println("-------------------------------------\n");

  delay(intervalo);
}
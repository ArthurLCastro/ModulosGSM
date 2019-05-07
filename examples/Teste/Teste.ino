#include <ModulosGSM.h>

// DEFINIÇÕES:
#define RX_GSM1 8         // ligar o Pino 8 do Arduino no TX do Módulo "SIM808 EVB-V3.2"
#define TX_GSM1 7         // ligar o Pino 7 do Arduino no RX do Módulo "SIM808 EVB-V3.2"
#define intervalo 5000    // Intervalo entre os envios de dados ao servidor

// CRIAÇÃO DE OBJETOS - Para emular as comunicações Seriais nos pinos digitais definidos acima:
ModulosGSM meuGSM1;
SoftwareSerial serialGSM1(RX_GSM1, TX_GSM1);

// DECLARAÇÕES DE VARIÁVEIS GLOBAIS:
  String url = "", envio = "";
  String sensor1, sensor2, sensor3;
  bool estadoEnvio;

void setup(){
  Serial.begin(9600);                       // Inicia a comunicação Serial a uma taxa de transmissão de 9600
  serialGSM1.begin(9600);                   // Inicia a comunicação Serial Emulada a uma taxa de transmissão de 9600
  meuGSM1.setupGSM(serialGSM1);
}

void loop(){
  sensor1 = "temp=" + String(lerTemperatura());
  sensor2 = "umid=" + String(lerUmidade());
  sensor3 = "lum=" + String(lerLuminosidade());

  url = "https://castroarthurelectronics.000webhostapp.com/get/add.php";
  envio = url + "?" + sensor1 + "&" + sensor2 + "&" + sensor3;

  estadoEnvio = meuGSM1.TestgetGSM(envio);

  Serial.print("estadoEnvio: ");
  Serial.println(estadoEnvio);

  delay(intervalo);
}

double lerTemperatura(){
  // Desenvolver a função de leitura do sensor
  double s1 = 28.6;
  return s1;
}

double lerUmidade(){
  // Desenvolver a função de leitura do sensor
  double s2 = 15.3;
  return s2;
}

double lerLuminosidade(){
  // Desenvolver a função de leitura do sensor
  double s3 = 94.6;
  return s3;
}
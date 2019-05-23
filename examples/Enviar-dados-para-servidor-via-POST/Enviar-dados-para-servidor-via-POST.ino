// Enviar Dados para um Servidor Web via POST
// Arthur L. Castro
// Maio de 2019

// INCLUSÃO DE BIBLIOTECAS:
#include <ModulosGSM.h>

// DEFINIÇÕES:
#define RX_GSM1 8         // ligar o Pino 8 do Arduino no TX do Módulo "SIM808 EVB-V3.2"
#define TX_GSM1 7         // ligar o Pino 7 do Arduino no RX do Módulo "SIM808 EVB-V3.2"
#define intervalo 10000   // Intervalo entre os envios de dados ao servidor

// CRIAÇÃO DE OBJETOS - Para emular as comunicações Seriais nos pinos digitais definidos acima:
ModulosGSM meuGSM1;
SoftwareSerial serialGSM1(RX_GSM1, TX_GSM1);

// DECLARAÇÕES DE VARIÁVEIS GLOBAIS:
  String url = "", pacote = "";
  static String contentType = "application/x-www-form-urlencoded";
  bool conSegura = 1;       // '1' para conexões HTTPS ou '0' para conexões HTTP
  bool estadoEnvio;
  String sensor1, sensor2, sensor3;

void setup(){
  Serial.begin(9600);                       // Inicia a comunicação Serial a uma taxa de transmissão de 9600
  serialGSM1.begin(9600);                   // Inicia a comunicação Serial Emulada a uma taxa de transmissão de 9600
  meuGSM1.setupGSM(serialGSM1);
}

void loop(){
  sensor1 = "temp=" + String(lerTemperatura());
  sensor2 = "umid=" + String(lerUmidade());
  sensor3 = "lum=" + String(lerLuminosidade());

// Se a página for http (conSegura = 0), deve-se pôr a parte "http://" na URL a ser enviada
  url = "https://castroarthurelectronics.000webhostapp.com/post/add.php";
  pacote = sensor1 + "&" + sensor2 + "&" + sensor3;

  estadoEnvio = meuGSM1.httpWritePOST(url, conSegura, contentType, pacote);

  Serial.print("estadoEnvio: ");
  Serial.println(estadoEnvio);

  delay(intervalo);
}

double lerTemperatura(){
  // Desenvolver a função de leitura do sensor
  double s1 = 26.2;
  return s1;
}

double lerUmidade(){
  // Desenvolver a função de leitura do sensor
  double s2 = 17.1;
  return s2;
}

double lerLuminosidade(){
  // Desenvolver a função de leitura do sensor
  double s3 = 30.23;
  return s3;
}
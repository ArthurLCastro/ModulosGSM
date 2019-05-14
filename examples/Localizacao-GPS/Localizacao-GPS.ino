// Exemplo 006 - Captura de Localização via GPS
// Arthur L. Castro
// Maio de 2019

/*
   Este código tem como objetivo...
   
   Mais de um Módulo pode ser conectado ao microcontrolador pois a comunicação
   Serial pode ser feita a partir de pinos digitais utilizando a biblioteca
   "SoftwareSerial", padrão do Arduino.
   
   Observação (Módulo SIM808 EVB-V3.2):
   Este módulo possui um pushbutton para habilitar o GSM. Para que haja um
   funcionamento adequado deve-se pressioná-lo e verificar os LEDs de indicação
   na placa após a alimentação do sistema. Também é possível refazer os pontos de
   solda curto-circuitando o pushbutton.
   Lembrete:
   No caso de você querer utilizar a comunicação Serial física do Arduino,
   não será possível ver as respostas recebidas do Módulo no MonitorSerial.
*/


// INCLUSÃO DE BIBLIOTECAS:
#include <ModulosGSM.h>

// DEFINIÇÕES:
#define RX_GSM1 8         // ligar o Pino 8 do Arduino no TX do Módulo "SIM808 EVB-V3.2"
#define TX_GSM1 7         // ligar o Pino 7 do Arduino no RX do Módulo "SIM808 EVB-V3.2"
#define estabilizarGPS 5000
#define intervalo 5000

// CRIAÇÃO DE OBJETOS - Para emular as comunicações Seriais nos pinos digitais definidos acima:
ModulosGSM meuGSM1;
SoftwareSerial serialGSM1(RX_GSM1, TX_GSM1);

// DECLARAÇÕES DE VARIÁVEIS GLOBAIS:
//String latitude = "", longitude = "", altitude = "", data = "", hora = "";
String info = "";

void setup(){
  Serial.begin(9600);                       // Inicia a comunicação Serial a uma taxa de transmissão de 9600
  serialGSM1.begin(9600);                   // Inicia a comunicação Serial Emulada a uma taxa de transmissão de 9600
  meuGSM1.setupGSM(serialGSM1);

  meuGSM1.powerGPS(true);               // Habilita a alimentação do GPS
//  Serial.println("[DEBUG] Aguardando estabilizacao do GPS...");
//  delay(estabilizarGPS);                // Tempo para estabilizar a conexão do GPS
}

void loop(){
  info = meuGSM1.infoGPS();
//  longitude = meuGSM1.longitudeGPS();
//  altitude = meuGSM1.altitudeGPS();
//  data = meuGSM1.dataGPS();
//  hora = meuGSM1.horaGPS();

//  meuGSM1.powerGPS(false);               // Desabilita a alimentação do GPS
/*
  Serial.println("-------------------------------------");
  Serial.println("|     Localizacao do Modulo GSM     |");
  Serial.println("-------------------------------------");
  Serial.print("\t> Latitude: ");
  Serial.println(latitude);
  Serial.print("\t> Longitude: ");
  Serial.println(longitude);
  Serial.print("\t> Altitude: ");
  Serial.println(altitude);
  Serial.print("\t> Data: ");
  Serial.println(data);
  Serial.print("\t> Hora: ");
  Serial.println(hora);
  Serial.println("-------------------------------------\n");
*/
  delay(intervalo);
}
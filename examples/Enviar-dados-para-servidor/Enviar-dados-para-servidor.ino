// Exemplo 003 - Enviar Dados para um Servidor Web via GET
// Arthur L. Castro
// Abril de 2019

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
#define intervalo 5000    // Intervalo entre os envios de dados ao servidor

// CRIAÇÃO DE OBJETOS - Para emular as comunicações Seriais nos pinos digitais definidos acima:
ModulosGSM meuGSM1;
SoftwareSerial serialGSM1(RX_GSM1, TX_GSM1);

// DECLARAÇÕES DE VARIÁVEIS GLOBAIS:
//String pagina;
  String url = "", envio = "";
  String sensor1, sensor2, sensor3;
  bool estadoEnvio;

void setup(){
  Serial.begin(9600);                       // Inicia a comunicação Serial a uma taxa de transmissão de 9600
  serialGSM1.begin(9600);                   // Inicia a comunicação Serial Emulada a uma taxa de transmissão de 9600
  meuGSM1.setupGSM(serialGSM1);
}

void loop(){
//  pagina = meuGSM1.lerDadosWebGSM();
//  Serial.println(pagina);

  sensor1 = "temp=" + String(lerTemperatura());
  sensor2 = "umid=" + String(lerUmidade());
  sensor3 = "lum=" + String(lerLuminosidade());

//  url = "Insira aqui a URL do seu servidor";
  url = "https://castroarthurelectronics.000webhostapp.com/get/add.php";
  envio = url + "?" + sensor1 + "&" + sensor2 + "&" + sensor3;

  estadoEnvio = meuGSM1.httpWriteGET(envio);
/*
  if (estadoEnvio == 1){
    Serial.println("[DEBUG] Dados enviados ao Servidor com sucesso!");
  } else {
    Serial.println("[DEBUG] Erro no envio de dados ao Servidor!");  
  }
*/

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
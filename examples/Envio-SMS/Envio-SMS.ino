// Exemplo 005 - Envio de SMS
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
#define intervalo 20000

// CRIAÇÃO DE OBJETOS - Para emular as comunicações Seriais nos pinos digitais definidos acima:
ModulosGSM meuGSM1;
SoftwareSerial serialGSM1(RX_GSM1, TX_GSM1);

// DECLARAÇÕES DE VARIÁVEIS GLOBAIS:
String numero = "Insira_aqui_o_numero_que_receberá_o_SMS";    // Define o numero que receberá o SMS
String msg = "Mensagem SMS enviada do Modulo GSM!";           // Define a Mensagem a ser transmitida
bool estadoSMS = false;                                       // Indica se a Mensagem foi enviada com Sucesso

void setup(){
  Serial.begin(9600);                       // Inicia a comunicação Serial a uma taxa de transmissão de 9600
  serialGSM1.begin(9600);                   // Inicia a comunicação Serial Emulada a uma taxa de transmissão de 9600
  meuGSM1.setupGSM(serialGSM1);
}

void loop(){
  estadoSMS = meuGSM1.enviarSMS(numero, msg);
  
  if(estadoSMS == true){
    Serial.println("Mensagem enviada para " + numero + " :");
    Serial.println("  -> " + msg);
  } else {
    Serial.println("[ERRO] Erro ao enviar SMS para " + numero);
  }
  
  delay(intervalo);         // Intevalo entre envios
}
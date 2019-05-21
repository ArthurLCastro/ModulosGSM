# Biblioteca ModulosGSM

## Descrição:
Desenvolvimento da Biblioteca **ModulosGSM** para utilização com Sistemas Embarcados.

Mais de um Módulo pode ser conectado ao microcontrolador pois a comunicação Serial pode ser feita a partir de pinos digitais utilizando a biblioteca "SoftwareSerial", padrão do Arduino.

_**Lembrete:**
</br>
No caso de você utilizar a comunicação Serial física do Arduino para se comunicar com o Módulo GSM, não será possível ver as respostas recebidas do Módulo no MonitorSerial._

_**Importante (Módulo SIM808 EVB-V3.2):**
</br>
Este módulo possui um pushbutton para habilitar o GSM. Para que haja um funcionamento adequado deve-se pressioná-lo e verificar os LEDs de indicação na placa após a alimentação do sistema. Também é possível refazer os pontos de solda curto-circuitando o pushbutton._

## Módulos em Teste:
* SIM808 EVB-V3.2
* GSM GPRS SIM800L (Não possui GPS)

## Códigos de Exemplo:
### Teste de Conexão com o Módulo
Este código tem como objetivo testar a conexão entre o microcontrolador e o Módulo GSM.
</br>
Código: [Teste-de_Conexao.ino](https://github.com/ArthurLCastro/ModulosGSM/blob/master/examples/Teste-de-Conexao/Teste-de-Conexao.ino)

### Efetuar Chamada de Voz
Descrição ainda em Desenvolvimento

### Enviar SMS
Descrição ainda em Desenvolvimento

### Receber SMS
Descrição ainda em Desenvolvimento

### Captura de Localização via GPS
Descrição ainda em Desenvolvimento

### Enviar Dados para um Servidor Web via GET
Descrição ainda em Desenvolvimento

### Ler uma Página Web via GET
Descrição ainda em Desenvolvimento

### Enviar Dados para um Servidor Web via POST
Descrição ainda em Desenvolvimento

## Funções da Biblioteca **ModulosGSM**:
### **setupGSM**
* Ação:</br>
	Configura o Módulo GSM
* Parâmetros:</br>
	Stream
* Retorno:</br>
	Nenhum.

### **testeConexaoGSM**
* Ação:</br>
	Verifica se a conexão entre o Módulo GSM e o Microcontrolador está ativa.
* Parâmetros:</br>
	Nenhum
* Retorno:</br>
	Nenhum.

### ...
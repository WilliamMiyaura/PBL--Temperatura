# PBL--Temperatura
Trabalho de N2 para o 5º Semestre do curso de Engenharia da Computação
# Projeto MQTT com ESP32

Este projeto utiliza um ESP32 para conectar-se a uma rede Wi-Fi e a um broker MQTT, permitindo o controle de um LED onboard e a leitura de temperatura via sensores analógicos.

## Funcionalidades

- Conexão à rede Wi-Fi especificada.
- Conexão a um broker MQTT.
- Controle do LED onboard via comandos MQTT.
- Leitura de temperatura de um sensor analógico e envio dos dados para o broker MQTT.

## Hardware Necessário

- ESP32
- Sensor de temperatura analógico (conectado ao pino 35)

## Bibliotecas Utilizadas

- [WiFi](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)
- [PubSubClient](https://github.com/knolleary/pubsubclient)

## Configurações

As seguintes configurações devem ser ajustadas no código conforme necessário:

const char* default_SSID = "William_Net"; // Nome da rede Wi-Fi
const char* default_PASSWORD = "william2004"; // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "20.51.218.228"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/lamp007/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/lamp007/attrs"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_2 = "/TEF/lamp007/attrs/l"; // Tópico MQTT de envio de informações para Broker
const char* default_ID_MQTT = "fiware_007"; // ID MQTT
const int default_D4 = 2; // Pino do LED onboard

## Estrutura do Código
# Inicialização
initSerial(): Inicializa a comunicação serial.
initWiFi(): Configura a conexão Wi-Fi.
initMQTT(): Configura o broker MQTT.
# Configuração do Hardware
InitOutput(): Configura o pino do LED onboard.
# Funções Principais
setup(): Função de configuração inicial.
loop(): Função principal que roda continuamente.
# Conexão
reconectWiFi(): Verifica e reconecta ao Wi-Fi, se necessário.
reconnectMQTT(): Verifica e reconecta ao broker MQTT, se necessário.
# Callbacks
mqtt_callback(char* topic, byte* payload, unsigned int length): Trata mensagens recebidas pelo MQTT.
# Verificação e Envio de Estados
VerificaConexoesWiFIEMQTT(): Verifica as conexões Wi-Fi e MQTT.
EnviaEstadoOutputMQTT(): Envia o estado do LED para o broker MQTT.
# Leitura de Sensores
readTemperature(): Lê o valor do sensor de temperatura e envia ao broker MQTT.
# Uso
Clone este repositório para sua máquina local.
Abra o projeto no Arduino IDE.
Modifique as configurações Wi-Fi e MQTT conforme necessário.
Carregue o código no seu ESP32.
Monitore a saída serial para verificar as mensagens de debug e o estado das conexões.


Licença
Este projeto é licenciado sob a MIT License.


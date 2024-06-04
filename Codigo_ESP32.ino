#include <WiFi.h>
#include <PubSubClient.h>

// Configurações - variáveis editáveis
const char* default_SSID = "William_Net"; // Nome da rede Wi-Fi
const char* default_PASSWORD = "william2004"; // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "20.51.218.228"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/lamp007/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/lamp007/attrs"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_2 = "/TEF/lamp007/attrs/l"; // Tópico MQTT de envio de informações para Broker
const char* default_ID_MQTT = "fiware_007"; // ID MQTT
const int default_D4 = 2; // Pino do LED onboard
// Declaração da variável para o prefixo do tópico
const char* topicPrefix = "lamp007";

// Variáveis para configurações editáveis
char* SSID = const_cast<char*>(default_SSID);
char* PASSWORD = const_cast<char*>(default_PASSWORD);
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE);
char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1);
char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2);
char* ID_MQTT = const_cast<char*>(default_ID_MQTT);
int D4 = default_D4;

// Instâncias de cliente Wi-Fi e MQTT
WiFiClient espClient;
PubSubClient MQTT(espClient);
char EstadoSaida = '0'; // Estado inicial do LED

// Inicializa a comunicação serial
void initSerial() {
  Serial.begin(115200);
}

// Conecta à rede Wi-Fi
void initWiFi() {
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");
  reconectWiFi();
}

// Configura o cliente MQTT e define o callback
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

// Função inicial chamada ao iniciar o dispositivo
void setup() {
  InitOutput(); // Configura a saída (LED)
  initSerial(); // Inicializa a comunicação serial
  initWiFi(); // Conecta ao Wi-Fi
  initMQTT(); // Configura o MQTT
  delay(5000); // Espera 5 segundos
  MQTT.publish(TOPICO_PUBLISH_1, "s|on"); // Publica uma mensagem inicial
}

// Loop principal
void loop() {
  VerificaConexoesWiFIEMQTT(); // Verifica e reconecta se necessário
  EnviaEstadoOutputMQTT(); // Envia o estado do LED
  readTemperature(); // Lê e envia a temperatura
  MQTT.loop(); // Mantém o cliente MQTT ativo
}

// Conecta-se ao Wi-Fi se não estiver conectado
void reconectWiFi() {
  if (WiFi.status() == WL_CONNECTED)
    return;
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado com sucesso na rede ");
  Serial.print(SSID);
  Serial.println("IP obtido: ");
  Serial.println(WiFi.localIP());

  // Garante que o LED inicie desligado
  digitalWrite(D4, LOW);
}

// Callback que lida com mensagens recebidas via MQTT
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    msg += c;
  }
  Serial.print("- Mensagem recebida: ");
  Serial.println(msg);

  // Forma o padrão de tópico para comparação
  String onTopic = String(topicPrefix) + "@on|";
  String offTopic = String(topicPrefix) + "@off|";

  // Compara com o tópico recebido
  if (msg.equals(onTopic)) {
    digitalWrite(D4, HIGH);
    EstadoSaida = '1';
  }

  if (msg.equals(offTopic)) {
    digitalWrite(D4, LOW);
    EstadoSaida = '0';
  }
}

// Verifica as conexões Wi-Fi e MQTT
void VerificaConexoesWiFIEMQTT() {
  if (!MQTT.connected())
    reconnectMQTT();
  reconectWiFi();
}

// Envia o estado do LED ao broker MQTT
void EnviaEstadoOutputMQTT() {
  if (EstadoSaida == '1') {
    MQTT.publish(TOPICO_PUBLISH_1, "s|on");
    Serial.println("- Led Ligado");
  }

  if (EstadoSaida == '0') {
    MQTT.publish(TOPICO_PUBLISH_1, "s|off");
    Serial.println("- Led Desligado");
  }
  Serial.println("- Estado do LED onboard enviado ao broker!");
  delay(1000);
}

// Configura o pino do LED e faz um piscar inicial
void InitOutput() {
  pinMode(D4, OUTPUT);
  digitalWrite(D4, HIGH);
  boolean toggle = false;

  for (int i = 0; i <= 10; i++) {
    toggle = !toggle;
    digitalWrite(D4, toggle);
    delay(200);
  }
}

// Tenta reconectar ao broker MQTT
void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      MQTT.subscribe(TOPICO_SUBSCRIBE);
    } else {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Haverá nova tentativa de conexão em 2s");
      delay(2000);
    }
  }
}

// Lê a temperatura de um sensor e envia via MQTT
void readTemperature() {
  int temperaturePin = 35;
  int valorSensor = analogRead(temperaturePin);
  float temperatura = (valorSensor * (100.0 / 4095.0)) + 7.2;
  char mensagem[10];
  snprintf(mensagem, 10, "%.2f", temperatura);
  MQTT.publish(TOPICO_PUBLISH_2, mensagem);
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" Graus Celsius");
  Serial.print(valorSensor);
}

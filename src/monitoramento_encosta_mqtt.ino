#include <Wire.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <PubSubClient.h>

MPU6050 mpu;

// WIFI
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// BROKER MQTT
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

// PINOS
#define SOIL_PIN 34

#define RED 25
#define GREEN 26
#define BLUE 27

#define BUZZER 14

float baselineAngle = 0;

// FUNÇÃO MPU6050
float getAngle() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  return atan2(ax, az) * 180 / PI;
}

// RGB
void setColor(bool r, bool g, bool b) {
  digitalWrite(RED, r);
  digitalWrite(GREEN, g);
  digitalWrite(BLUE, b);
}

// WIFI
void setup_wifi() {

  Serial.println("Conectando ao WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.println(WiFi.localIP());
}

// MQTT
void reconnect() {

  while (!client.connected()) {

    Serial.println("Conectando ao broker MQTT...");

    String clientId = "ESP32-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {

      Serial.println("MQTT conectado!");

    } else {

      Serial.print("Falha MQTT. Codigo: ");
      Serial.println(client.state());

      delay(2000);
    }
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Wire.begin();
  mpu.initialize();

  setup_wifi();

  client.setServer(mqtt_server, 1883);

  Serial.println("Inicializando sistema...");
  delay(2000);

  baselineAngle = getAngle();

  Serial.print("Angulo base: ");
  Serial.println(baselineAngle);

  Serial.println("Sistema iniciado!");
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  // UMIDADE
  int soilValue = analogRead(SOIL_PIN);
  float humidity = map(soilValue, 0, 4095, 100, 0);

  // INCLINACAO
  float angle = getAngle();
  float variation = abs(angle - baselineAngle);

  String statusRisco;

  // LOGICA
  if (humidity >= 80 || variation >= 8) {

    setColor(1,0,0);
    tone(BUZZER, 2000);

    statusRisco = "RISCO_CRITICO";

  }
  else if ((humidity >= 60 && humidity < 80) ||
           (variation >= 5 && variation < 8)) {

    setColor(1,1,0);
    tone(BUZZER, 1000, 200);

    statusRisco = "ATENCAO";

  }
  else {

    setColor(0,1,0);
    noTone(BUZZER);

    statusRisco = "SEGURO";
  }

  // SERIAL
  Serial.print("Umidade: ");
  Serial.print(humidity);

  Serial.print("% | Variacao angular: ");
  Serial.print(variation);

  Serial.print(" | Status: ");
  Serial.println(statusRisco);

  // JSON MQTT
  String payload = "{";
  payload += "\"umidade\":" + String(humidity) + ",";
  payload += "\"inclinacao\":" + String(variation) + ",";
  payload += "\"status\":\"" + statusRisco + "\"";
  payload += "}";

  client.publish("encosta/dados", payload.c_str());

  delay(1000);
}

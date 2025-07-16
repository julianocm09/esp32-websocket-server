#include <WiFi.h>
#include <ESP32MQTTClient.h>
#include <ArduinoJson.h>

// WiFi
const char* ssid = "granjavortmann_EXT";
const char* password = "1020304050";

// MQTT HiveMQ Cloud
const char* mqttHost = "8475554a27dc4860b4e751f39c31d384.s2.eu.hivemq.cloud";
const int mqttPort = 8883;
const char* mqttUser = "julianomangold";      // substitua pelo seu usuário HiveMQ
const char* mqttPassword = "125487Asd";   // substitua pela sua senha HiveMQ

const char* topicSubscribe = "esp32/comando";
const char* topicPublish = "esp32/status";

const int led18Pin = 18;
const int led19Pin = 19;

void onMessageReceived(String &topic, String &payload) {
  Serial.printf("[MQTT] Mensagem no tópico [%s]: %s\n", topic.c_str(), payload.c_str());

  if (payload == "led18 on") {
    digitalWrite(led18Pin, HIGH);
    Serial.println("[MQTT] LED18 ligado");
  } 
  else if (payload == "led18 off") {
    digitalWrite(led18Pin, LOW);
    Serial.println("[MQTT] LED18 desligado");
  } 
  else if (payload == "led19 on") {
    digitalWrite(led19Pin, HIGH);
    Serial.println("[MQTT] LED19 ligado");
  } 
  else if (payload == "led19 off") {
    digitalWrite(led19Pin, LOW);
    Serial.println("[MQTT] LED19 desligado");
  } 
  else if (payload == "info") {
    StaticJsonDocument<400> doc;
    doc["granja"] = "granja Adriano Vortmann";
    doc["board"] = "ESP32 maternidade 1 ID:" + String((uint32_t)ESP.getEfuseMac(), HEX);
    doc["temp_salaA1"] = "22°C";
    doc["temp_salaA2"] = "22°C";
    doc["temp_salaB1"] = "22°C";
    doc["temp_salaV2"] = "22°C";
    doc["temp_gestacao_s1"] = "22°C";          
    doc["temp_gestacao_s2"] = "22°C";          
    doc["temp_gestacao_s3"] = "22°C";
    doc["core_version"] = ESP.getSdkVersion();
    doc["chip_model"] = ESP.getChipModel();
    doc["cpu_freq_mhz"] = ESP.getCpuFreqMHz();
    doc["flash_size"] = ESP.getFlashChipSize();
    doc["led18"] = digitalRead(led18Pin) ? "ON" : "OFF";
    doc["led19"] = digitalRead(led19Pin) ? "ON" : "OFF";

    String json;
    serializeJson(doc, json);
    ESP32MQTTClient::publish(topicPublish, json);
    Serial.println("[MQTT] Informações enviadas");
  } 
  else {
    Serial.println("[MQTT] Comando desconhecido → ignorado");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(led18Pin, OUTPUT);
  pinMode(led19Pin, OUTPUT);

  // Configura WiFi e MQTT
  ESP32MQTTClient::WiFi = ssid;
  ESP32MQTTClient::WiFi_Password = password;

  ESP32MQTTClient::MQTT_Server = m_

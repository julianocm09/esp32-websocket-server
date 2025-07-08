#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

// Dados da sua rede Wi-Fi
const char* ssid = "Juliano_c5_2.4";
const char* password = "Theo11031601";

// Dados do seu servidor WebSocket (Render.com)
const char* websocket_host = "esp32-websocket-server-lrf5.onrender.com";
const uint16_t websocket_port = 443;  // SSL (https)
const char* websocket_path = "/";

WebSocketsClient webSocket;

// Pinos dos LEDs
const int led18Pin = 18;
const int led19Pin = 19;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("[WebSocket] Desconectado do servidor");
      break;

    case WStype_CONNECTED:
      Serial.printf("[WebSocket] Conectado ao servidor: %s\n", payload);
      // Não envia nenhuma mensagem aqui!
      break;

    case WStype_TEXT: {
      String msg = String((char*)payload);
      msg.trim();

      Serial.printf("[WebSocket] Mensagem recebida: %s\n", msg.c_str());

      if (msg == "led18 on") {
        digitalWrite(led18Pin, HIGH);
        Serial.println("[WebSocket] LED 18 ligado");
      } 
      else if (msg == "led18 off") {
        digitalWrite(led18Pin, LOW);
        Serial.println("[WebSocket] LED 18 desligado");
      } 
      else if (msg == "led19 on") {
        digitalWrite(led19Pin, HIGH);
        Serial.println("[WebSocket] LED 19 ligado");
      } 
      else if (msg == "led19 off") {
        digitalWrite(led19Pin, LOW);
        Serial.println("[WebSocket] LED 19 desligado");
      } 
      else if (msg == "info") {
        StaticJsonDocument<200> doc;
        doc["led18"] = digitalRead(led18Pin) ? "ON" : "OFF";
        doc["led19"] = digitalRead(led19Pin) ? "ON" : "OFF";

        String json;
        serializeJson(doc, json);
        webSocket.sendTXT(json);
        Serial.println("[WebSocket] Informações enviadas");
      } 
      else {
        Serial.println("[WebSocket] Comando desconhecido → ignorado");
        // Não responde nada
      }
      break;
    }

    case WStype_BIN:
      Serial.printf("[WebSocket] Binário recebido (%u bytes)\n", length);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando ESP32 WebSocket Client");

  pinMode(led18Pin, OUTPUT);
  pinMode(led19Pin, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");

  webSocket.beginSSL(websocket_host, websocket_port, websocket_path);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void loop() {
  webSocket.loop();
}

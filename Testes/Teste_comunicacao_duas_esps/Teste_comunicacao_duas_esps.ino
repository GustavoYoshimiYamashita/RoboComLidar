const int ledPin = 5;  // Pino D5 onde o LED está conectado

#include <WiFi.h>        // Usamos a biblioteca WiFi.h para ESP32
#include <HTTPClient.h>  // Usamos a biblioteca HTTPClient.h para ESP32
#include <ArduinoJson.h>
#include <Wire.h>
int distance;

long T;
float distanceCM;

const char* ssid = "yanashita2.4";
const char* password = "yama1306";
const char* serverIP = "192.168.1.102";
const int serverPort = 8080;
const char* updateEndpoint = "/updateDistance";

int distancia;

void setup() {
  pinMode(ledPin, OUTPUT);  // Define o pino do LED como saída
  Serial.begin(115200);     // Inicializa a comunicação serial
                            //digitalWrite(ledPin, HIGH);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('.');  // Lê até encontrar o ponto
    int pos = data.indexOf(',');                // Encontra a posição da vírgula


    if (pos != -1) {
      String anguloStr = data.substring(0, pos);      // Obtém a parte do ângulo
      String distanciaStr = data.substring(pos + 1);  // Obtém a parte da distância
      int angulo = anguloStr.toInt();                 // Converte para inteiro
      distancia = distanciaStr.toInt();               // Converte para inteiro

      digitalWrite(ledPin, HIGH);
      delay(1);
      digitalWrite(ledPin, LOW);

      // Acende o LED para indicar recebimento de dados
      // Agora você pode usar o ângulo e a distância como desejar
      // Exemplo: Serial.println("Ângulo: " + String(angulo) + ", Distância: " + String(distancia));

      if (WiFi.status() == WL_CONNECTED) {
        StaticJsonDocument<200> doc;
        doc["distance"] = distancia;
        Serial.println(distancia);
        String payload;
        serializeJson(doc, payload);

        String url = "http://" + String(serverIP) + ":" + String(serverPort) + updateEndpoint;

        WiFiClient client;
        HTTPClient http;
        http.begin(client, url);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(payload);

        if (httpResponseCode == HTTP_CODE_OK) {
          String response = http.getString();
        } else {
          Serial.println("HTTP request failed");
        }

        http.end();
      }
    }
  }
}

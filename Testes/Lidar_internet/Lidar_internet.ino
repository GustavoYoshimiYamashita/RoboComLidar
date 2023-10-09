#include <WiFi.h>            // Usamos a biblioteca WiFi.h para ESP32
#include <HTTPClient.h>      // Usamos a biblioteca HTTPClient.h para ESP32
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

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
}

void loop() {
  
  if (WiFi.status() == WL_CONNECTED) {
    
    distance = 0;
    
    StaticJsonDocument<200> doc;
    doc["distance"] = distance;
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
  
  delay(1);
}


#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid = "yanashita2.4";
const char* password = "yama1306";

WiFiServer server(8080);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao WiFi");
  server.begin();
  Serial.println(WiFi.localIP());
}

void loop() {
  
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Novo cliente conectado");

    // Lê os dados do sensor
    int sensorData = lerSensor();

    // Envia os dados ao cliente
    client.print(sensorData);
    client.println();

    client.stop();
    Serial.println("Cliente desconectado");
  }
}

int lerSensor() {
  // Código para ler o sensor e obter os dados
  // Substitua esta função com a leitura real do seu sensor
  return random(0, 100);
}

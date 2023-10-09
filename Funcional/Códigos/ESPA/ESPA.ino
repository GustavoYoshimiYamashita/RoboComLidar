#include <WiFi.h>  // Usamos a biblioteca WiFi.h para ESP32
#include <WiFiClient.h>
#include <HTTPClient.h>  // Usamos a biblioteca HTTPClient.h para ESP32
#include <ArduinoJson.h>
#include <Wire.h>
int distance;

long T;
float distanceCM;

const char* ssid = "yanashita2.4";
const char* password = "yama1306";

WiFiServer server(8080);

const int ledPin = 2;  // Pino do LED
int contador = 0;

String receivedData = "";  // String para armazenar os dados recebidos

void setup() {
  Serial.begin(115200);     // Inicia a comunicação serial
  pinMode(ledPin, OUTPUT);  // Configura o pino do LED como saída

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao WiFi");
  server.begin();
  Serial.println(WiFi.localIP());
}

String lista_valores;
String valor_sensor_concatenado;

void loop() {
  int receivedNumber = readReceivedNumber();

  if (receivedNumber == 10) {
    // Significa que houve a quebra de dados
    int strLength = lista_valores.length();
    valor_sensor_concatenado = "";

    for (int i = 0; i < strLength; i += 2) {
      // Extrai os pares de caracteres
      String subString = lista_valores.substring(i, i + 2);

      // Transforma da tabela ascii em número
      int valor_sensor = subString.toInt();  // Correção: Usar toInt() para converter para int
      valor_sensor = valor_sensor - 48;
      valor_sensor_concatenado = valor_sensor_concatenado + String(valor_sensor);
    }

    //Serial.println(valor_sensor_concatenado);

    lista_valores = "";  // Limpa a string para os próximos dados
  } else if (receivedNumber == -1) {
    // Significa que não recebeu sinal
    // ignora
  } else {
    // Recebeu um dado
    lista_valores += String(receivedNumber);  // Adiciona o número recebido à lista_valores
  }

  if (receivedNumber != -1) {
    //Serial.print("Número recebido: ");
    //Serial.println(receivedNumber);
  } else {
    //Serial.println("Nenhum dado recebido!");
  }

  WiFiClient client = server.available();

  if (client) {
    // Envia os dados ao cliente
    client.print(valor_sensor_concatenado);
    client.println();

    client.stop();
  }
}

int readReceivedNumber() {
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();

    return receivedChar;

    // Se o caractere recebido for um dígito ou uma quebra de linha, adiciona à string
    if (isdigit(receivedChar) || receivedChar == '\n') {
      receivedData += receivedChar;
    } else {
      // Se não for um dígito nem uma quebra de linha, é o fim do número
      if (receivedData.length() > 0) {
        int receivedNumber = receivedData.toInt();  // Converte a string para um número inteiro
        receivedData = "";                          // Limpa a string para o próximo número
        return receivedNumber;
      }
    }
  }
  return -1;  // Retorna -1 se nenhum número foi completamente recebido
}

int receber_valor() {
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
    return receivedChar;
  }
  return -1;
}

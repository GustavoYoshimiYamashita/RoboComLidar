const int ledPin = 2;  // Pino do LED

void setup() {
  Serial.begin(9600);  // Inicia a comunicação serial
  pinMode(ledPin, OUTPUT);  // Configura o pino do LED como saída
}

void loop() {
  Serial.write('1');  // Envia '1' para ligar o LED
  delay(1000);
  Serial.write('0');  // Envia '0' para desligar o LED
  delay(1000);
}

//Definindo o sensor Hall
#define hall 7
bool validador = true;
bool boleano = 1;
// define números de pinos do driver no arduino
const int stepPin = 8;
const int dirPin = 9;
#include <SoftwareSerial.h> //header file of software serial port
SoftwareSerial Serial1(2, 3); //define software serial port name as Serial1 and define pin2 as RX and pin3 as TX
/* For Arduinoboards with multiple serial ports like DUEboard, interpret above two pieces of code and
  directly use Serial1 serial port*/
int distance; //actual distance measurements of LiDAR
int strength; //signal strength of LiDAR
float temprature;
int check; //save check value
int i;
int uart[9]; //save data measured by LiDAR
const int HEADER = 0x59; //frame header of data package

float pos = 0.0;
int angulo;

void setup () {
  //Define os pinos como entrada
  pinMode(hall, INPUT);
  // Define os dois pinos como Saídas
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, LOW);
  Serial.begin(115200); //set bit rate of serial port connecting Arduino with computer
  Serial1.begin(115200); //set bit rate of serial port connecting LiDAR with Arduino
  //pontoInicial();
}
void loop() {
  // permite que o motor se mova em uma direção particular
  
  digitalWrite(dirPin, HIGH);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(700);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(700);
  pos = pos + 0.666;
  if (pos > 200) {
    pos = 0;
  }
  //angulo = map(pos, 0, 200, 0, 360);
  //dist_calc(angulo);

}

void pontoInicial() {
  int leituraHall = digitalRead(hall);
  while (leituraHall == 1) {
    leituraHall = digitalRead(hall);
    digitalWrite(dirPin, boleano);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1500);
  }
  pos = 0;
  if (leituraHall == 0) {
    while (leituraHall != 1) {
      leituraHall = digitalRead(hall);
      pos++;
      digitalWrite(dirPin, boleano);
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000);
    }
    for (int i = 0; i <= pos / 2; i++) {
      leituraHall = digitalRead(hall);
      boleano = 0;
      digitalWrite(dirPin, boleano);
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000);
    }
    exit;
  }
}

float dist_calc(int angulo) {
  if (Serial1.available()) { //check if serial port has data input
    if (Serial1.read() == HEADER) { //assess data package frame header 0x59
      uart[0] = HEADER;
      if (Serial1.read() == HEADER) { //assess data package frame header 0x59
        uart[1] = HEADER;
        for (i = 2; i < 9; i++) { //save data in array
          uart[i] = Serial1.read();
        }
        check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
        if (uart[8] == (check & 0xff)) { //verify the received data as per protocol
          distance = uart[2] + uart[3] * 256; //calculate distance value
          Serial.print(angulo);
          Serial.print(",");
          Serial.print(distance);
          Serial.print(".");
        }
      }
    }
  }
}
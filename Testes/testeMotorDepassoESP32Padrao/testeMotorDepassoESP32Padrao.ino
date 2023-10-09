//Definindo o sensor Hall
//#define hall 7
bool validador = true;
bool boleano = 1;
// define números de pinos do driver no arduino
const int stepPin = 22;
const int dirPin = 23;

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

/* VARIÁVEIS DA INTERNET */
void setup () {
  //Define os pinos como entrada
  //pinMode(hall, INPUT);
  // Define os dois pinos como Saídas
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.begin(115200); //set bit rate of serial port connecting Arduino with computer
  Serial2.begin(115200); //set bit rate of serial port connecting LiDAR with Arduino
  //pontoInicial();
}

void loop() {

  //Serial.println("teste");
  // permite que o motor se mova em uma direção particular
  digitalWrite(dirPin, HIGH);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(700); // 1500
  digitalWrite(stepPin, LOW);
  delayMicroseconds(700);
  //delayMicroseconds(6200); // 1500
  pos = pos + 0.6666667;
  //Serial.print("pos: ");
  //Serial.println(pos);

  // Posicao lenta: 4200 rapido: 230
  int valor = 266;
  if (pos > valor) {
    pos = 0;
  }

  angulo = map(pos, 0, valor, 0, 360);
  dist_calc(pos);

}

/*void pontoInicial() {
  int leituraHall = digitalRead(hall);
  while (leituraHall == 1) {
    leituraHall = digitalRead(hall);
    digitalWrite(dirPin, boleano);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
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
}*/

void dist_calc(int angulo) {
  if (Serial2.available()) {
    if (Serial2.read() == HEADER) {
      uart[0] = HEADER;
      if (Serial2.read() == HEADER) {
        uart[1] = HEADER;
        for (i = 2; i < 9; i++) {
          uart[i] = Serial2.read();
        }
        check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
        if (uart[8] == (check & 0xff)) {
          distance = uart[2] + uart[3] * 256;
          Serial.print(angulo);
          Serial.print(",");
          Serial.println(distance);  // Alterei aqui para imprimir uma linha e usar 
        }
      }
    }
  }
}


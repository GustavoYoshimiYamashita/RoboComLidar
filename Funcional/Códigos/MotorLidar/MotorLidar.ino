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
unsigned long previousStepTime = 0;

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

unsigned long diferenca;

void loop() {
  unsigned long currentTime = micros();
  
  // permite que o motor se mova em uma direção particular
  digitalWrite(dirPin, HIGH);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(1200); // Atraso inicial (ajuste conforme necessário)
  digitalWrite(stepPin, LOW);

  // Chama a função e mede o tempo que ela leva para executar
  unsigned long startTime = micros();
  dist_calc();
  unsigned long elapsedTime = micros() - startTime;

  // Ajusta o atraso para compensar o tempo de execução da função
  int adjustedDelay = 1200 - elapsedTime;
  if (adjustedDelay > 0) {
    delayMicroseconds(adjustedDelay);
  }

  // Atualiza o tempo do passo anterior
  previousStepTime = currentTime;
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

void sendNumber(int number) {
  char buffer[10];  // Buffer para armazenar a representação do número
  sprintf(buffer, "%d", number);  // Converte o número em uma string
  // Envia cada dígito da string pela porta serial
  for (int i = 0; buffer[i] != '\0'; i++) {
    Serial.write(buffer[i]);
    delay(1);  // Ajuste conforme necessário para garantir que todos os caracteres sejam enviados
  }
  Serial.write('\n');  // Adiciona uma quebra de linha para indicar o fim do número
}


void dist_calc() {
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
          sendNumber(distance);
          Serial.println(distance);
        }
      }
    }
  }
}


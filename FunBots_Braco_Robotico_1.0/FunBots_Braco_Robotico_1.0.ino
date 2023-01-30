/*
      Programa para Controle Braço Robótico 3D FunBots

      Baseado nos vídeos:
        Robô Original: https://www.youtube.com/watch?v=66PGTT4eTzM
        Display OLED: https://youtu.be/8iJdbe6OP14

      Componentes:
        - Braço Robótico impresso 3D
        - 5 Servos MG90S
        - Arduino Nano
        - Display OLED SSD1306
        - Módulo DC-DC Buck LM2596
        - Botões tipo táctil e Potenciômetros tipo joystick

      Versão 1.0 - Versão inicial - 01/Fev/2023

 *    * Criado por Cleber Borges - FunBots - @cleber.funbots  *     *

      Instagram: https://www.instagram.com/cleber.funbots/
      Facebook: https://www.facebook.com/cleber.funbots
      YouTube: https://www.youtube.com/channel/UCKs2l5weIqgJQxiLj0A6Atw
      Telegram: https://t.me/cleberfunbots
*/

// Inclusão de Bibliotecas
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Servo.h>

// Declara objeto para display
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// Definição dos pinos de I/O
const int servo1Pin = 4;
const int servo2Pin = 5;
const int servo3Pin = 6;
const int servo4Pin = 7;
const int servo5Pin = 8;
const int pot1XPin = A0;
const int pot1YPin = A1;
const int pot2XPin = A2;
const int pot2YPin = A3;
const int pot1SWPin = 3;
const int pot2SWPin = 2;
const int SW1Pin = 10;
const int SW2Pin = 11;
const int SW3Pin = 12;

// Variáveis Pots
int pot1X = 0;
int pot1Y = 0;
int pot2X = 0;
int pot2Y = 0;
int numReads = 1;
int maxAngRate = 4;
int minAngRate = -4;

// Variáveis Botões
boolean pot1SW = false;
boolean pot2SW = false;
boolean SW1 = false;
boolean SW2 = false;
boolean SW3 = false;

// Declara objetos dos servos
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// Angulos Limites dos servos
int servo1PosMax = 180; // Anti-Horário
int servo1PosMin = 0;   //Horário

int servo2PosMax = 180; // Baixo
int servo2PosMin = 110; // Cima

int servo3PosMax = 140; // Baixo
int servo3PosMin = 10;  // Cima

int servo4PosMax = 170; // Baixo
int servo4PosMin = 10;  // Cima

int servo5PosMax = 120; // Abre
int servo5PosMin = 10;  // Fecha

// Variáveis de posição dos Servos
int servo1Pos = 10;
int servo2Pos = 170;
int servo3Pos = 30;
int servo4Pos = 10;
int servo5Pos = 110;
int posServo[5] = {servo1Pos, servo2Pos, servo3Pos, servo4Pos, servo5Pos}; // Usada no modo Exemplo

// Variáveis diversas
boolean flagAtualizaDisplay = false;
boolean flagExemplo = false;

void setup() {

  // Inicia Serial
  Serial.begin(115200);

  // Configura pinos de entrada
  pinMode(pot1SWPin, INPUT_PULLUP);
  pinMode(pot2SWPin, INPUT_PULLUP);
  pinMode(SW1Pin, INPUT_PULLUP);
  pinMode(SW2Pin, INPUT_PULLUP);
  pinMode(SW3Pin, INPUT_PULLUP);

  // Inicia Wire I2C
  Wire.begin();

  // Inicia Display no endereço 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Configuração básica do texto
  display.setTextColor(WHITE);
  display.setTextSize(1);

  // Inicia variáveis dos Servos
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);
  servo5.attach(servo5Pin);

  // Mensagem de inicialização e movimenta Servos para posição inicial
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Iniciando Servos");
  display.display();
  delay(200);
  servo1.write(servo1Pos);

  display.print(".");
  display.display();
  delay(200);
  servo2.write(servo2Pos);

  display.print(".");
  display.display();
  delay(200);
  servo3.write(servo3Pos);

  display.print(".");
  display.display();
  delay(200);
  servo4.write(servo4Pos);

  display.println(".");
  display.display();
  delay(200);
  servo5.write(servo5Pos);

  delay(1000);
  atualizaDisplay();

  flagExemplo = true;
}

void loop() {

  leituraBotoes();

  while (flagExemplo) {
    modoExemplo();
    servo1Pos = posServo[0];
    servo2Pos = posServo[1];
    servo3Pos = posServo[2];
    servo4Pos = posServo[3];
    servo5Pos = posServo[4];
    if (!flagExemplo) atualizaDisplay();
  }

  // Move Servo Base
  if (pot1X > 0 || pot1X < 0) {
    servo1Pos = servo1Pos + pot1X;
    if (servo1Pos >= servo1PosMax) servo1Pos = servo1PosMax;
    if (servo1Pos <= servo1PosMin) servo1Pos = servo1PosMin;
    servo1.write(servo1Pos);
    flagAtualizaDisplay = true;
  }

  // Move Servo Braço 1
  if (pot1Y > 0 || pot1Y < 0) {
    servo2Pos = servo2Pos - pot1Y;
    if (servo2Pos >= servo2PosMax) servo2Pos = servo2PosMax;
    if (servo2Pos <= servo2PosMin) servo2Pos = servo2PosMin;
    servo2.write(servo2Pos);
    flagAtualizaDisplay = true;
  }

  // Move Servo Braço 2
  if (pot2X > 0 || pot2X < 0) {
    servo3Pos = servo3Pos + pot2X;
    if (servo3Pos >= servo3PosMax) servo3Pos = servo3PosMax;
    if (servo3Pos <= servo3PosMin) servo3Pos = servo3PosMin;
    servo3.write(servo3Pos);
    flagAtualizaDisplay = true;
  }

  // Move Servo Braço 3
  if (pot2Y > 0 || pot2Y < 0) {
    servo4Pos = servo4Pos - pot2Y;
    if (servo4Pos >= servo4PosMax) servo4Pos = servo4PosMax;
    if (servo4Pos <= servo4PosMin) servo4Pos = servo4PosMin;
    servo4.write(servo4Pos);
    flagAtualizaDisplay = true;
  }

  if (SW1) {
    servo5Pos++;
    if (servo5Pos >= servo5PosMax) servo5Pos = servo5PosMax;
    if (servo5Pos <= servo5PosMin) servo5Pos = servo5PosMin;
    servo5.write(servo5Pos);
  } else if (SW2) {
    servo5Pos--;
    if (servo5Pos >= servo5PosMax) servo5Pos = servo5PosMax;
    if (servo5Pos <= servo5PosMin) servo5Pos = servo5PosMin;
    servo5.write(servo5Pos);
  }

  // Checa se tudo está estabilizado e houve movimento anterior para atualizar display
  if (pot1X == 0 && pot1Y == 0 && pot2X == 0 && pot2Y == 0 && !SW1 && !SW2 && flagAtualizaDisplay) {
    atualizaDisplay();
    flagAtualizaDisplay = false;
  }

  /*
    // Exibe leituras via Serial para Apoio
    Serial.print("pot1X = ");
    Serial.print(pot1X);
    Serial.print(" ; pot1Y = ");
    Serial.print(pot1Y);
    Serial.print(" ; pot2X = ");
    Serial.print(pot2X);
    Serial.print(" ; pot2Y = ");
    Serial.print(pot2Y);
    Serial.print(" ; pot1SW = ");
    Serial.print(pot1SW);
    Serial.print(" ; pot2SW = ");
    Serial.print(pot2SW);
    Serial.print(" ; SW1 = ");
    Serial.print(SW1);
    Serial.print(" ; SW2 = ");
    Serial.print(SW2);
    Serial.print(" ; SW3 = ");
    Serial.println(SW3);
  */
}

// Função para Leitura dos botões
void leituraBotoes() {

  pot1X = 0;
  pot1Y = 0;
  pot2X = 0;
  pot2Y = 0;

  for (int i = 0 ; i < numReads ; i++) {
    pot1X += analogRead(pot1XPin);
    delay(5);
  }
  pot1X = pot1X / numReads;
  pot1X = map(pot1X, 0, 1023, minAngRate, maxAngRate);

  for (int i = 0 ; i < numReads ; i++) {
    pot1Y += analogRead(pot1YPin);
    delay(5);
  }
  pot1Y = pot1Y / numReads;
  pot1Y = map(pot1Y, 0, 1023, minAngRate, maxAngRate);

  for (int i = 0 ; i < numReads ; i++) {
    pot2X += analogRead(pot2XPin);
    delay(5);
  }
  pot2X = pot2X / numReads;
  pot2X = map(pot2X, 0, 1023, minAngRate, maxAngRate);

  for (int i = 0 ; i < numReads ; i++) {
    pot2Y += analogRead(pot2YPin);
    delay(5);
  }
  pot2Y = pot2Y / numReads;
  pot2Y = map(pot2Y, 0, 1023, minAngRate, maxAngRate);

  pot1SW = !digitalRead(pot1SWPin);
  pot2SW = !digitalRead(pot1SWPin);
  SW1 = !digitalRead(SW1Pin);
  SW2 = !digitalRead(SW2Pin);
  SW3 = !digitalRead(SW3Pin);

}

// Função para atualizar display
void atualizaDisplay() {

  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("Braco Robotico");

  display.print("S1:");
  display.print(servo1Pos);
  display.setCursor(44, 8);
  display.print("S2:");
  display.println(servo2Pos);

  display.print("S3:");
  display.print(servo3Pos);
  display.setCursor(44, 16);
  display.print("S4:");
  display.println(servo4Pos);

  display.print("S5:");
  display.print(servo5Pos);
  display.display();
}

void modoExemplo() {
  int servo1Ex1 = 10;
  int servo1Ex2 = 170;
  int servo2Ex1 = 130;
  int servo2Ex2 = 100;
  int servo3Ex1 = 130;
  int servo3Ex2 = 30;
  int servo4Ex1 = 105;
  int servo4Ex2 = 10;
  int servo5Ex1 = 30;
  int servo5Ex2 = 120;
  int vel = 5;
  int intervalo = 200;

  // Move para posição 1
  moveServo(1, servo1Ex1, vel);
  delay(intervalo);

  moveServo(3, servo3Ex1, vel);
  delay(intervalo);

  moveServo(4, servo4Ex1, vel);
  delay(intervalo);

  moveServo(5, servo5Ex1, vel);
  delay(intervalo);

  moveServo(4, servo4Ex2, vel);
  delay(intervalo);

  moveServo(3, servo3Ex2, vel);
  delay(intervalo);

  // Move para posição 2
  moveServo(1, servo1Ex2, vel);
  delay(intervalo);

  moveServo(3, servo3Ex1, vel);
  delay(intervalo);

  moveServo(4, servo4Ex1, vel);
  delay(intervalo);

  moveServo(5, servo5Ex2, vel);
  delay(intervalo);

  moveServo(4, servo4Ex2, vel);
  delay(intervalo);

  moveServo(3, servo3Ex2, vel);
  delay(intervalo);
}

void moveServo(int numServo, int newPosServo, int velocidade) {

  if (posServo[numServo - 1] < newPosServo) {
    for (posServo[numServo - 1]; posServo[numServo - 1] < newPosServo; posServo[numServo - 1]++) {
      servoWrite(numServo, posServo[numServo - 1]);
      delay(velocidade);
    }
  } else {
    for (posServo[numServo - 1]; posServo[numServo - 1] > newPosServo; posServo[numServo - 1]--) {
      servoWrite(numServo, posServo[numServo - 1]);
      delay(velocidade);
    }
  }

  atualizaDisplayExemplo();
  leituraBotoes();
  if (SW3) {
    flagExemplo = false;
  }
}

void servoWrite(int numServo, int pos) {
  switch (numServo) {
    case 1:
      servo1.write(pos);
      break;
    case 2:
      servo2.write(pos);
      break;
    case 3:
      servo3.write(pos);
      break;
    case 4:
      servo4.write(pos);
      break;
    case 5:
      servo5.write(pos);
      break;
    default:
      break;
  }
}

void atualizaDisplayExemplo() {

  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.print("Modo Exemplo");

  if (flagExemplo) {
    display.println(" ON");
  } else {
    display.println(" OFF");
  }

  display.print("S1:");
  display.print(posServo[0]);
  display.setCursor(44, 8);
  display.print("S2:");
  display.println(posServo[1]);

  display.print("S3:");
  display.print(posServo[2]);
  display.setCursor(44, 16);
  display.print("S4:");
  display.println(posServo[3]);

  display.print("S5:");
  display.print(posServo[4]);
  display.display();
}

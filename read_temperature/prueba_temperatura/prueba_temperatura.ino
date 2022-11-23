#include <math.h>
#define Vcc 5
#define R10K 10000
#define pinSensorAnalog 0
#define time_delay 1000

float k = 2.5;

// Definimos A, B y C del modelo Stenhart - Hart
float A = 1.11492089e-3;
float B = 2.372075385e-4;
float C = 6.954079529e-8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  float raw, V, R, logR, R_th, T_kelvin, T_celsius;
  // Obtenemos el valor de la resistencia a partir de la lectura de la tensión sobre ella:
  raw = analogRead(pinSensorAnalog);
  V = (raw / 1024) * Vcc;
  R = (R10K * V) / (Vcc - V);
  logR = log(R);

  // Modelo de Stenhart - Hart
  R_th = 1.0 / (A + B * logR + C * logR * logR * logR);

  // Compensación de autocalentamiento
  T_kelvin = R_th - ((V * V)/(k * R));
  T_celsius = T_kelvin - 273.15;

  // Imprimimos el resultado por pantalla
  Serial.print("T = ");
  Serial.print(T_celsius);
  Serial.print(" C\n");
  delay(time_delay);
}




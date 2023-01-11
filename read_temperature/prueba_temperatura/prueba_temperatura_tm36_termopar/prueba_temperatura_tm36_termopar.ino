/* 
Lectura sensor termistor + tmp36
https://www.prometec.net/sensor-tmp36/


- Montaje.
A0: tm36 directamente a la salida analógica del sensor  (10 mV por grado)
A1: termistor con resistencia en serie 10koh
*/



#include <math.h>
#define Vcc 5.0
#define R10K 10000
#define pinTermistor 0
#define pinSensorTM36 1 
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
  float raw, V, R, logR, R_th, T_kelvin, T_celsius, T_tm36;
  // Obtenemos el valor de la resistencia a partir de la lectura de la tensión sobre ella:
  raw = analogRead(pinTermistor);
  T_tm36 =  (Vcc * analogRead(pinSensorTM36) / 1024) * 100 - 50; // 10 mV por grado, además le tenemos que restar 50, porque el cero es -50
  V = (raw / 1024) * Vcc;
  R = (R10K * V) / (Vcc - V);
  logR = log(R);

  // Modelo de Stenhart - Hart
  R_th = 1.0 / (A + B * logR + C * logR * logR * logR);

  // Compensación de autocalentamiento
  T_kelvin = R_th - ((V * V)/(k * R));
  T_celsius = T_kelvin - 273.15;

  // Imprimimos el resultado por pantalla
  Serial.print("T. termi = ");
  Serial.print(T_celsius);
  Serial.print(" C ");
  Serial.print("T. tm36 = ");
  Serial.print(T_tm36);
  Serial.print(" C\n");
  delay(time_delay);
}




/* 
Lectura sensor DS18B20.
Ejemplo extraído de https://naylampmechatronics.com/blog/46_tutorial-sensor-digital-de-temperatura-ds18b20.html
También: https://programarfacil.com/blog/arduino-blog/ds18b20-sensor-temperatura-arduino/


- Montaje.
Es un sensor que utiliza el protocolo 1 - WIRE para comunicarse y solo necesita un pin de datos para comunicarse y permite conectar más de un sensro en el mismo bus
Necesitamos una resistencia de 4k7 ohms entre masa y la salida digital.

- Características.
Resolución programable de 9 hasta 12 bits.
Cada sensor tiene una dirección única de 64 bits.

- librerías necesarias:
-- One - wire: https://github.com/milesburton/Arduino-Temperature-Control-Library
-- DallasTemperature: https://github.com/milesburton/Arduino-Temperature-Control-Library
Para más información de la libreria, mirar el siguiente enlace: http://www.milesburton.com/Dallas_Temperature_Control_Library

CONFIGURACIÓN PUERTOS ARDUINO
-----------------------------

DIGITALES:

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
  T_tm36 =  (Vcc * analogRead(pinSensorTM36) / 1024) * 100 - 50;
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




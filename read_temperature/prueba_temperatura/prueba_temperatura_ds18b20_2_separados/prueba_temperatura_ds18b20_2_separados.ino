/* 
Lectura de dos sensores de temperatura DS18B20 .
Ejemplo extraído de https://naylampmechatronics.com/blog/46_tutorial-sensor-digital-de-temperatura-ds18b20.html (este es con el que estoy trabajando)
También: https://programarfacil.com/blog/arduino-blog/ds18b20-sensor-temperatura-arduino/

- Montaje.
Es un sensor que utiliza el protocolo 1 - WIRE para comunicarse y solo necesita un pin de datos para comunicarse y permite conectar más de un sensro en el mismo bus
Necesitamos una resistencia de 4k7 ohms entre masa y la salida digital.

- Características.
Resolución programable de 9 hasta 12 bits.
Cada sensor tiene una dirección única de 64 bits.

- Librerías necesarias:
-- One - wire: https://github.com/milesburton/Arduino-Temperature-Control-Library
-- DallasTemperature: https://github.com/milesburton/Arduino-Temperature-Control-Library
Para más información de la libreria, mirar el siguiente enlace: http://www.milesburton.com/Dallas_Temperature_Control_Library

CONFIGURACIÓN PUERTOS ARDUINO
-----------------------------

DIGITALES:
2 : sensor con cápsula acero inoxidable.
3 : sensor sin cápsula de acero inoxidable.
*/
#include <TimeLib.h> // https://github.com/PaulStoffregen/Time
#include <OneWire.h>
#include <DallasTemperature.h>
//#define time_delay 1000 // La temporización no es exacta, tener esto en cuenta. Si colocamos aquí un siete, son ocho segundos de espera entre medida aprox. (segundos)

OneWire ourWire1(2); // el pin2 será el bus OneWire
OneWire ourWire2(3);
DallasTemperature sensor1(&ourWire1);
DallasTemperature sensor2(&ourWire2);
const unsigned long time_delay = 1000;

void setup() {
  Serial.begin(9600);
  sensor1.begin();
  sensor2.begin();
  //setTime();
}

void loop() {
  time_t t = now();
  sensor1.requestTemperatures(); // Se envía comando para leer la temperatura
  float temp1 = sensor1.getTempCByIndex(0);

  sensor2.requestTemperatures(); // Se envía comando para leer la temperatura
  float temp2 = sensor2.getTempCByIndex(0); 

  imprimir(temp1, temp2, t);
  delay(time_delay);
}

void imprimir(float temp1, float temp2, time_t t){
  Serial.print(t);
  Serial.print("s. ");
  //Serial.print(minute(t));
  Serial.print(" T1 = ");
  Serial.print(temp1);
  Serial.print(" C ");
  Serial.print("T2 = ");
  Serial.print(temp2);
  Serial.print(" C\n");  
}



/* 
Lectura de solo un sensor DS18B20 de temperatura.
Ejemplo extraído de https://naylampmechatronics.com/blog/46_tutorial-sensor-digital-de-temperatura-ds18b20.html (este es con el que estoy trabajando)
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
2 : sensor con cápsula acero inoxidable.

*/
#include <OneWire.h>
#include <DallasTemperature.h>
#define time_delay 2000

OneWire ourWire(2); // el pin2 será el bus OneWire
DallasTemperature sensors(&ourWire);

void setup() {

  Serial.begin(9600);
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures(); // Se envía comando para leer la temperatura
  float temp = sensors.getTempCByIndex(0);
  Serial.print("T = ");
  Serial.print(temp);
  Serial.print(" C\n");
  delay(4000);

}




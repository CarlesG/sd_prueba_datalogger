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
2 : sensor con cápsula acero inoxidable (cable de 3 metros). + sensor sin capsula + sensor acero inoxidable (con cable más corto)
*/

#include <TimeLib.h> // https://github.com/PaulStoffregen/Time
#include <OneWire.h>
#include <DallasTemperature.h>
//#define time_delay 1000 // La temporización no es exacta, tener esto en cuenta. Si colocamos aquí un siete, son ocho segundos de espera entre medida aprox. (segundos)
const unsigned long time_delay = 1000;

OneWire ourWire(2); // el pin2 será el bus OneWire
DallasTemperature sensors(&ourWire);
DeviceAddress address_inox1 = {0x28, 0x82, 0xA6, 0x18, 0x0, 0x0, 0x0, 0x11};
DeviceAddress address_ambiente = {0x28, 0xB5, 0xAB, 0x16, 0xA8, 0x1, 0x3C, 0x8A};
DeviceAddress address_inox2 = {0x28, 0x64, 0xA3, 0x75, 0x3C, 0x19, 0x1, 0x30};


void setup() {
  delay(1000);
  Serial.begin(9600);
  sensors.begin();
 } 
  

void loop() {
  sensors.requestTemperatures(); // Se envía comando para leer la temperatura
  float temp_inox1 = sensors.getTempC(address_inox1);
  float temp_inox2 = sensors.getTempC(address_inox2);
  float temp_ambiente = sensors.getTempC(address_ambiente);  



  imprimir(temp_inox1, temp_inox2, temp_ambiente);
  delay(time_delay);
}

void imprimir(float temp1, float temp2, float temp3){
  
  Serial.print(" Tinox1 = ");
  Serial.print(temp1);
  Serial.print(" C");
  Serial.print(" Tinox2 = ");
  Serial.print(temp2);
  Serial.print(" C");
  Serial.print(" Tambiente = ");
  Serial.print(temp3);
  Serial.print(" C\n");   
}



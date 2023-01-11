

/* 
ESTE PROGRAMA SIRVE PARA AVERIGUAR LA DIRECCIÓN DE CADA SENSOR, CONECTADO AL PIN CORRESPONDIENTE ONE - WIRE
En este caso el pin 2 digital.

- Librerías necesarias:
-- One - wire: https://github.com/milesburton/Arduino-Temperature-Control-Library
-- DallasTemperature: https://github.com/milesburton/Arduino-Temperature-Control-Library
Para más información de la libreria, mirar el siguiente enlace: http://www.milesburton.com/Dallas_Temperature_Control_Library

CONFIGURACIÓN PUERTOS ARDUINO
-----------------------------

DIGITALES:
2 : sensor con cápsula acero inoxidable. y sensor sin capsula DS18B20
*/

#include <OneWire.h>           //https://github.com/PaulStoffregen/OneWire
#include <DallasTemperature.h> //https://github.com/milesburton/Arduino-Temperature-Control-Library

OneWire ourWire(4); // el pin2 será el bus OneWire

void setup() {
  Serial.begin(115200);
}

void loop() {
  byte addr[8]; // array de 8 bytes, recordar que son direcciones de 64 bits que son 8 bytes 
  Serial.println("Obteniendo direcciones:");
  while(ourWire.search(addr)){
    Serial.print("Address = ");
    for(int i = 0; i < 8 ; i++){
      Serial.print(" 0x");
      Serial.print(addr[i], HEX);
    }
    Serial.println();
  }
  Serial.println();
  ourWire.reset_search();
  delay(2000);
}

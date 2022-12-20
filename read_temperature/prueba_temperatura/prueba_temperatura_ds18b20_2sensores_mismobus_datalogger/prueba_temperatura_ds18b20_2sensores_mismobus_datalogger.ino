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
7:  pin digital 7 para el parpadeo del led.
ANALÓGICOS:
0 : divisor de tensión que aguanta máximo 25 V. El factor máximo es de 5, con la configuración de las resistencias usadas en el divisor.
*/
#include <avr/sleep.h>
#include "LowPower.h"
//#include "ArduinoLowPower.h"
#include <DS1307RTC.h>
#include <TimeLib.h>  // https://github.com/PaulStoffregen/Time
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#define ANALOG_IN_PIN A0

File myFile;
String time;  // String of data
const unsigned long time_sleep = 240; // time en seconds
const int chipSelect = 10;
tmElements_t tm;  // Structure to read RTC fields
OneWire ourWire(2);  // el pin2 será el bus OneWire
DallasTemperature sensors(&ourWire);

//DeviceAddress address_inox1 = {0x28, 0x82, 0xA6, 0x18, 0x0, 0x0, 0x0, 0x11};
DeviceAddress address_ambiente = { 0x28, 0xB5, 0xAB, 0x16, 0xA8, 0x1, 0x3C, 0x8A };
//DeviceAddress address_inox2 = {0x28, 0x64, 0xA3, 0x75, 0x3C, 0x19, 0x1, 0x30};
DeviceAddress address_red = { 0x28, 0x7F, 0x92, 0x16, 0x0, 0x0, 0x0, 0xA7 };
DeviceAddress address_blue = { 0x28, 0xED, 0xB2, 0x18, 0x0, 0x0, 0x0, 0x95 };

const int ledPin = 3;

float volatile in_voltage = 0.0;
int counts_voltage = 0;
float const R1 = 30000.0;
float const R2 = 7500.0;

void setup() {
  //pinMode(7, OUTPUT);  // pin 7 for output of led blink
 
  Serial.begin(9600);
  delay(2000);
  sensors.begin();
  Serial.println("ArduinoAll DataLogger Shield Test with temperature sensors and tension input");
  pinMode(SS, OUTPUT);

  if (!SD.begin(chipSelect)) {
    Serial.println("SD Card initialization failed!");
    return;
  }
  Serial.println("SD Card OK.");
}

void loop() {
  float temp_red, temp_blue, temp_ambiente;
  int n_times = 3;
 
  counts_voltage = analogRead(ANALOG_IN_PIN);
  in_voltage = counts_voltage * ((R1 + R2) / R2) * (5 / 1024.0);
 
  sensors.requestTemperatures();  // Se envía comando para leer la temperatura
  sensors.setResolution(12);  // Set the resolution for the sensors. (9,10,11 and 12 bits)
  
  // READ SENSORS AND TIME AND CREATE STRING FOR WRITE IN SD
  read_sensors(&temp_red, &temp_blue, &temp_ambiente);
  time = Now(temp_red, temp_blue, temp_ambiente, in_voltage);
  Serial.println(time);
  WriteText(time);
  blink(n_times);

  // ARDUINO SLEEP MODE
  sleep(time_sleep);
  delay(1000);
}
/*
--------------------------------------------- 
Print the temperature sensors lectures
---------------------------------------------
*/
void imprimir(float temp1, float temp2, float temp3) {
  Serial.print(" Tred = ");
  Serial.print(temp1);
  Serial.print(" C");
  Serial.print(" Tblue = ");
  Serial.print(temp2);
  Serial.print(" C");
  Serial.print(" Tambiente = ");
  Serial.print(temp3);
  Serial.print(" C\n");
}

/*-------------------------------------------
Function for read the temperature sensors:
  * INPUTS (reference of float variables)
  -------------------------------------------
    temp_red (reference)
    temp_blue (reference)
    temp_ambiente (reference)
---------------------------------------------
*/
void read_sensors(float *temp_red, float *temp_blue, float *temp_ambiente) {
  *temp_red = sensors.getTempC(address_red);
  *temp_blue = sensors.getTempC(address_blue);
  *temp_ambiente = sensors.getTempC(address_ambiente);
}

/* Function for write the information in a line.
  * INPUT
  -------
    String txt:
*/
void WriteText(String txt) {

  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    myFile.println(txt);
    myFile.close();
    //myFile.sync();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

/*
-------------------------------------------------------------------
Function for write in a string the date and the temperature sensors
-------------------------------------------------------------------
*/
String Now(float temp_red, float temp_blue, float temp_ambiente, float voltage) {
  String time = "";
  String sep = ";";
  if (RTC.read(tm)) {
    // DATE ----------------------------
    if (tm.Day / 10 == 0) {
      time += "0" + (String)tm.Day;
    } else {
      time += tm.Day;
    }

    if (tm.Month / 10 == 0) {
      time += "0" + (String)tm.Month;
    } else {
      time += tm.Month;
    }
    time += tmYearToCalendar(tm.Year);
    time += sep;

    // HOUR ------------------------------
    if (tm.Hour / 10 == 0) {
      time += "0" + (String)tm.Hour;
    } else {
      time += tm.Hour;
    }
    if (tm.Minute / 10 == 0) {
      time += "0" + (String)tm.Minute;
    } else {
      time += tm.Minute;
    }
    if (tm.Second / 10 == 0) {
      time += "0" + (String)tm.Second;
    } else {
      time += tm.Second;
    }
  } else {
    delay(1500);
    if (RTC.read(tm)) {
      // DATE ----------------------------
      if (tm.Day / 10 == 0) {
        time += "0" + (String)tm.Day;
      } else {
        time += tm.Day;
      }

      if (tm.Month / 10 == 0) {
        time += "0" + (String)tm.Month;
      } else {
        time += tm.Month;
      }
      time += tmYearToCalendar(tm.Year);
      time += sep;

      // HOUR ------------------------------
      if (tm.Hour / 10 == 0) {
        time += "0" + (String)tm.Hour;
      } else {
        time += tm.Hour;
      }
      if (tm.Minute / 10 == 0) {
        time += "0" + (String)tm.Minute;
      } else {
        time += tm.Minute;
      }
      if (tm.Second / 10 == 0) {
        time += "0" + (String)tm.Second;
      } else {
        time += tm.Second;
      }
    } else {
      time = "000000;000000";
      if (RTC.chipPresent()) {
        Serial.println("The DS1307 is stopped.  Please run the SetTime");
        Serial.println("example to initialize the time and begin running.");
        //Serial.println();
      } else {
        Serial.println("DS1307 read error!  Please check the circuitry.");
        //Serial.println();
      }
    }
  }
  time += sep;
  // Sensor values to add to time string.
  time += temp_red;
  time += sep;
  time += temp_blue;
  time += sep;

  time += temp_ambiente;
  time += sep;
  time += voltage;
  return time;
}

/*
---------------------------------------------------
Function for read the time. return it with a String.
  OUTPUT:
    String: returns the date with hour in the format
      (YYYYMMDD_HHMMSS)
-----------------------------------------------------
*/
String read_date() {
  String date = "";
  String sep = "_";
  if (RTC.read(tm)) {
    // DATE ----------------------------
    if (tm.Day / 10 == 0) {
      time += "0" + (String)tm.Day;
    } else {
      time += tm.Day;
    }

    if (tm.Month / 10 == 0) {
      time += "0" + (String)tm.Month;
    } else {
      time += tm.Month;
    }
    time += tmYearToCalendar(tm.Year);
    time += sep;

    // HOUR -----------------------------
    if (tm.Hour / 10 == 0) {
      time += "0" + (String)tm.Hour;
    } else {
      time += tm.Hour;
    }
    if (tm.Minute / 10 == 0) {
      time += "0" + (String)tm.Minute;
    } else {
      time += tm.Minute;
    }
    if (tm.Second / 10 == 0) {
      time += "0" + (String)tm.Second;
    } else {
      time += tm.Second;
    }
  } else {
    date = "error";
  }
  return date;
}

/*Function that blinks ntimes  */
void blink(int ntimes) {
  for (int i = 0; i < ntimes; i++){
    digitalWrite(ledPin, HIGH);
    delay(250);
    digitalWrite(ledPin, LOW);
    delay(500);
    }
}

void sleep(int sec) {
  while (sec >= 8) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    //LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
    sec -= 8;
  }
  if (sec >= 4) {
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
    //LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
    sec -= 4;
  }
  if (sec >= 2) {
    LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
    //LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
    sec -= 2;
  }
  if (sec >= 1) {
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
    //LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
    sec -= 1;
  }
 // sendSignal();
}

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
#include <DS1307RTC.h>
#include <TimeLib.h> // https://github.com/PaulStoffregen/Time
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
//#define time_delay 1000 // La temporización no es exacta, tener esto en cuenta. Si colocamos aquí un siete, son ocho segundos de espera entre medida aprox. (segundos)
const unsigned long time_delay = 3000;


File myFile;
const int chipSelect = 10;
String time ; // String of data
tmElements_t tm; // Structure to read RTC fields

OneWire ourWire(2); // el pin2 será el bus OneWire
DallasTemperature sensors(&ourWire);
DeviceAddress address_inox1 = {0x28, 0x82, 0xA6, 0x18, 0x0, 0x0, 0x0, 0x11};
DeviceAddress address_ambiente = {0x28, 0xB5, 0xAB, 0x16, 0xA8, 0x1, 0x3C, 0x8A};
DeviceAddress address_inox2 = {0x28, 0x64, 0xA3, 0x75, 0x3C, 0x19, 0x1, 0x30};
DeviceAddress address_red = {0x28, 0x7F, 0x92, 0x16, 0x0, 0x0, 0x0, 0xA7};
DeviceAddress address_blue = {0x28, 0xED, 0xB2, 0x18, 0x0, 0x0, 0x0, 0x95};


void setup() {
  delay(1000);
  Serial.begin(9600);
  sensors.begin();
  Serial.println("ArduinoAll DataLogger Shield Test");
  pinMode(SS, OUTPUT);

  if (!SD.begin(chipSelect)) {
    Serial.println("SD Card initialization failed!");
    return;  
  }
  Serial.println("SD Card OK.");
 } 
  
void loop() {
  sensors.requestTemperatures(); // Se envía comando para leer la temperatura
  //float temp_inox1 = sensors.getTempC(address_inox1);
  //float temp_inox2 = sensors.getTempC(address_inox2);
  float temp_red;
  float temp_blue;
  float temp_ambiente;
  sensors.setResolution(12); // Set the resolution for the sensors. (9,10,11 and 12 bits)
  read_sensors(&temp_red, &temp_blue, &temp_ambiente);
  imprimir(temp_red, temp_blue, temp_ambiente);
  time = Now();
  Serial.println(time);
  WriteText(time);
  delay(time_delay);
}

void imprimir(float temp1, float temp2, float temp3){
  
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

void read_sensors(float *temp_red, float *temp_blue, float *temp_ambiente){
  *temp_red = sensors.getTempC(address_red);
  *temp_blue = sensors.getTempC(address_blue);
  *temp_ambiente = sensors.getTempC(address_ambiente);  
}

void WriteText(String txt){
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    myFile.println(txt);
    myFile.close();
  } 
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

String Now(){
  String time = "";
  String sep = ";"; 
  if (RTC.read(tm)) {
    // DATE ----------------------------
    if (tm.Day / 10 == 0 ){
      time += "0" + (String) tm.Day;
    }else{
      time += tm.Day;
    }
    
    if (tm.Month / 10 == 0){
      time += "0" + (String) tm.Month;
    }
    else{
      time += tm.Month;
    }
    time += tmYearToCalendar(tm.Year);
    time += sep;

    // HOUR ------------------------------
    if (tm.Hour / 10 == 0){
      time += "0" + tm.Hour;
    }else{
      time += tm.Hour;      
    }
    if (tm.Minute / 10 == 0){
      time += "0" + (String) tm.Minute;
    }else{
      time += tm.Minute;
    }
    if (tm.Second / 10 == 0){
      time += "0" + (String) tm.Second;
    }else{
      time += tm.Second;
    }
    time += sep;
    // Sensor values to add to time string.
  } 
  else {
    time = "NO";
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } 
    else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
  }
  return time;
}



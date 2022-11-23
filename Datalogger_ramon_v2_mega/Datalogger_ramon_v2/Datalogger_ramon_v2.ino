/* MEGA IMPLEMENTATION 
  SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10

 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

We insert another dispositive, the az-delivery datalogger, that use other libraries
 https://www.az-delivery.de/es/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/daten-logger-shield-am-mega-r3

 */

#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
const int analog_read_freq = 3;  // Wait between acquisitions in seconds
unsigned long timestamp=0;  // Timestamp

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(10, 11, 12, 13)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  // Check to see if the file exists and remove it:
  if (SD.exists("datalog.txt")) {
    SD.remove("datalog.txt"); Serial.println("Removed previous file...");
  }
  
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
 // if the file is available, write to it:
  if (dataFile) {
    dataFile.println("sep=;\r");
    dataFile.println("Time ; Voltage ; Intensity");
    dataFile.close();
    // print to the serial port too:
    Serial.println("Time ; Voltage ; Intensity");
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
   
}

void loop() {
  // make a string for assembling the data to log:
  String dataString = "";

  // read Sensor 0 Voltage
   int sensor = analogRead(0);
  // read Sensor 1 Intensity
   int sensor1 = analogRead(1);
   dataString += String(timestamp);dataString += ";";dataString += String(sensor);
   dataString += ";";dataString += String(sensor1);
   timestamp+=analog_read_freq;  
      


  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
   delay(analog_read_freq*1000);              // wait for the next acquisition
}

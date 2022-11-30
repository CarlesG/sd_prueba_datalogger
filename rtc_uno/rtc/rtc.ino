/* USE IT IN ARDUINO UNO 

THIS PROGRAM WRITES IN THE SD CARD EACH time_write VALUE.

SET-UP.
All sensor must go on digital pin 3, because we use ONE-WIRE library.

Extracted from: 
https://www.instructables.com/Tutorial-How-to-Use-Arduino-Data-Logger-Shield-to-/
https://www.youtube.com/watch?v=wVKZpOUuCzw

libraries:
https://drive.google.com/file/d/1BlB2s_8aj9bOoKwivhwXBeexrAMOIwqH/view

*/ 
#include <DS1307RTC.h>
#include <TimeLib.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

File myFile;
const int chipSelect = 10;
const int time_write = 3000; // each time we want to write on SD card

String time ; // String of data
tmElements_t tm; // Structure to read RTC fields


void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("ArduinoAll DataLogger Shield Test");
  pinMode(SS, OUTPUT);

  if (!SD.begin(chipSelect)) {
    Serial.println("SD Card initialization failed!");
    //time = Now() + " Sensor Value";
    //Serial.println(time);
    return;  
  }
  Serial.println("SD Card OK.");
  //ReadText();
}

void loop() {
  time = Now();
  Serial.println(time);
  WriteText(time);
  delay(time_write);
}

/*
void ReadText(){
  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } 
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
*/

/* Function to write a line in the file 
  INPUT: txt String variable with the info to write

*/
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

/* Function for write the date and hour of each lecture in sensors 
*/
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

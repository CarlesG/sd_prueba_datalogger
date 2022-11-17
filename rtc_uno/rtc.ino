#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

File myFile;
const int chipSelect = 10;

String time ;
tmElements_t tm;


void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("ArduinoAll DataLogger Shield Test");
  pinMode(SS, OUTPUT);

  if (!SD.begin(chipSelect)) {
    Serial.println("SD Card initialization failed!");
     time = Now()+" Sensor Value";
  Serial.println(time);
    return;  
  }
  Serial.println("SD Card OK.");
  ReadText();
}

void loop() {
  time = Now()+" Sensor Value";
  Serial.println(time);
  WriteText("prueba");
  delay(3000);
}


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
  if (RTC.read(tm)) {
    //    time = String(tm.Hour+":"+tm.Minute+":"+tm.Secnd+" DAY : "+tm.Day+"/"+tm.Month+"/"+tmYearToCalendar(tm.Year));
    time+=tm.Hour;
    time+=":";

    time+=tm.Minute;
    time+=":";

    time+=tm.Second;
    time+=" DAY : ";

    time+=tm.Day;
    time+="/";

    time+=tm.Month;
    time+="/";

    time+=tmYearToCalendar(tm.Year);
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

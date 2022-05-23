const int VIN = A0;
#include <ACS712_30A.h>
#include <millisDelay.h>
unsigned long delayStart = 0; 
bool delayRunning = false; 
int i = 0;
float f = 0;
const float ii = 0.01666667;
unsigned long previousMillis=0; 
float Current = 0;
float Voltage = 0;
float KiloVolt = 0;
float KWH = 0;
float Bill = 0;
String Relay_status = "ON";
ACS712_30A project(VIN);
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "SD.h"
#include <Wire.h>
#include "RTClib.h"
#define LOG_INTERVAL  1000
#define SYNC_INTERVAL 1000 
uint32_t syncTime = 0; 
RTC_DS1307 RTC; 
const int chipSelect = 10;
File myfile;

void setup() 
{
    initSDcard();
    createFile();
    initRTC();
    myfile.println("millis,stamp,date,time,Current,Voltage(in W),Voltage(in KW),Operating Time in mins,Operating time in hrs,Units Consumed,Bill Amount,Relay Status");
    pinMode(7, OUTPUT);
    delayStart = millis();
    delayRunning = true;
    lcd.begin(); // initialize the lcd
    lcd.backlight();
    lcd.print("----Welcome----"); 
}

void loop() 
{
    if (delayRunning && ((millis() - delayStart) >= 60000)) 
  {
      delayStart = delayStart + 60000;
      i++;
      (f = i * ii);
  }
  
  if (((unsigned long)(millis() - previousMillis)) >= 1000) 
  {
      previousMillis = millis();
      Current = project.getCurrent();
      Voltage = project.getVoltage();
      KiloVolt = project.getKiloVolt();
      KWH = project.getKWh(f);
      Bill = project.getBill(f);

      if(Voltage >= 150)
      {
        digitalWrite(7, HIGH);
        Relay_status = "OFF";
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("WARNING !!");
        lcd.setCursor(1,0);
        lcd.print("OVERlOAD !!");
      }

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(String("Bill = ") + Bill);

      DateTime now;
      delay((LOG_INTERVAL - 1) - (millis() % LOG_INTERVAL));
      uint32_t m = millis(); 
      myfile.print(m);       
      myfile.print(", ");
      now = RTC.now();
      myfile.print(now.unixtime());
      myfile.print(", ");
      myfile.print(now.year(), DEC);
      myfile.print("/");
      myfile.print(now.month(), DEC);
      myfile.print("/");
      myfile.print(now.day(), DEC);
      myfile.print(" ");
      myfile.print(", ");
      myfile.print(now.hour(), DEC);
      myfile.print(":");
      myfile.print(now.minute(), DEC);
      myfile.print(":");
      myfile.print(now.second(), DEC);
      myfile.print(", ");
      myfile.print(Current);
      myfile.print(", ");
      myfile.print(Voltage);
      myfile.print(", ");
      myfile.print(KiloVolt);
      myfile.print(", ");
      myfile.print(i);
      myfile.print(", ");
      myfile.print(f);
      myfile.print(", ");
      myfile.print(KWH);
      myfile.print(", ");
      myfile.print(Bill);
      myfile.print(", ");
      myfile.println(Relay_status);

      if ((millis() - syncTime) < SYNC_INTERVAL) return;
      syncTime = millis();
      myfile.flush();
   }
}


void initSDcard()
  {
    pinMode(10, OUTPUT);
    if (!SD.begin(chipSelect)) 
    {
      return;
    }
  }

void createFile()
{
  char filename[] = "MLOG00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[4] = i / 10 + '0';
    filename[5] = i % 10 + '0';
    if (! SD.exists(filename)) {
      myfile = SD.open(filename, FILE_WRITE);
      break;
    }
  }
}

void initRTC()
{
  Wire.begin();
  if (!RTC.begin()) {
    myfile.println("RTC failed");
  }
}

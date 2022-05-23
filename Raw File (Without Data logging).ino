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

void setup() 
{
    Serial.begin(9600);
    pinMode(7, OUTPUT);
    delayStart = millis();
    delayRunning = true;
    Serial.println("ACS712-30A Current Sensor");
    Serial.println("Assumed, Electricity Cost per Unit = 5");
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
      
      Serial.print(String("Current: ") + Current + " A  ");
      Serial.print(String("Voltage: ") + Voltage + " W   ");
      Serial.print(String("(In KiloVolt): ") + KiloVolt + " KW)  ");
      Serial.print(String("Time (in mins): ") + i + " Time (in hrs): " + f + "  ");
      Serial.print(String("Units: ") + KWH + " KWH  ");
      Serial.print(String("Bill: ") + Bill + " Rupees  ");
      Serial.println(Relay_status);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(String("Bill = ") + Bill);
   }
}

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
# include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
 
//LCD
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

#define REPORTING_PERIOD_MS     1000
 
PulseOximeter pox;
uint32_t tsLastReport = 0;
 
void onBeatDetected()
{
    Serial.println("Beat!");
}

JsonDocument doc;
 
void setup()
{
    Serial.begin(115200);
    Serial.print("Initializing pulse oximeter..");
    
    lcd.init();
    lcd.backlight();
 
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
}
 
void loop()
{
    // Make sure to call update as fast as possible
    BP();
    lcd.setCursor(0,0);
    lcd.print("BPM : ");
    lcd.setCursor(7,0);
    lcd.print(pox.getHeartRate());
    lcd.setCursor(0,1);
    lcd.print("SpO2 : ");
    lcd.setCursor(8,1);
    lcd.print(pox.getSpO2());    
    delay(100);
}

void BP(){
  pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
        doc["BP"] = pox.getHeartRate();
        doc["sp"] = pox.getSpO2();
 
        tsLastReport = millis();
    }}

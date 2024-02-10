# include <TinyGPS++.h>
# include <SoftwareSerial.h>

int rxP = 7;
int txP = 6;

TinyGPSPlus gps;
SoftwareSerial SerialGPS(rxP, txP);

void setup() {
  Serial.begin(9600);
  SerialGPS.begin(9600);
}

void loop() {
  while(SerialGPS.available()>0)
    if (gps.encode(SerialGPS.read())){
      data();
    }
    if (millis()>5000 && gps.charsProcessed() < 10)
    {
      Serial.println("GPS Not Detected!!!");
      while(true);
    }
  }

void data()
{
  if (gps.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  }
  else
  {
    Serial.println("Location is not available");
  }
  
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(5000);
}
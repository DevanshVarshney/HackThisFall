# include <TinyGPS++.h>
# include <HardwareSerial.h>
# include <LiquidCrystal_I2C.h>
# include <OneWire.h>
# include <DallasTemperature.h>
# include <Wire.h>
# include <MAX30100_PulseOximeter.h>
# include <ArduinoJson.h>
# include <WiFi.h>
# include <PubSubClient.h>

//MQTT
const char* ssid = "Galaxy";
const char* password = "Trinetra";
const char* mqttServer = "192.168.204.126";
const int mqttPort = 1883;
const char* mqttUser = "test";
const char* mqttPassword = "123456789";
const char* mqttTopic = "trial/esp";5
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.println("Message arrived in topic: " + String(topic));
    Serial.println("Message: " + String((char*)payload));}

//GPS PINS
int RXPin = 16;
int TXPin = 17;
int GPSBaud = 9600;
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

//Temperature
const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

LiquidCrystal_I2C lcd(0x27, 16,2);  

JsonDocument root;

void setup() {
    Serial.begin(115200); // For debugging purposes
  gpsSerial.begin(GPSBaud, SERIAL_8N1 , RXPin,TXPin);
  lcd.init();
  lcd.backlight();

  sensors.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to MQTT broker
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  pinMode(19,INPUT);
}  
  


void loop() {
    // Read data from the GPS module
  String jsonStr;
  if (!client.connected()) {
    reconnect();
    }

    while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
    {
      gpsDisplayInfo();
      root["latitude"] = gps.location.lat();
      root["longitude"] = gps.location.lng();
      root["altitude"] = gps.altitude.meters();
      root["hour"] = gps.time.hour();
      root["minute"] = gps.time.minute();
      root["second"] = gps.time.second();
      root["day"] = gps.date.day();
      root["month"] = gps.date.month();
      root["year"] = gps.date.year();
      root["temperature"] = temp();
      if (digitalRead(19) != 0){
        root["bpm"] = random(85,90);
        root["spO2"] = random(98,100);
      }
      else{
        root["bpm"] = random(55,60);
        root["spO2"] = random(70,80);
      }
      
      serializeJson(root, jsonStr);
      Serial.println(jsonStr);
      client.loop();
      // Publish a string message to the MQTT topic
      client.publish(mqttTopic, jsonStr.c_str());

    // Wait for a while before publishing again
    delay(1000);      
    }
    if (millis() > 1000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
    
  }



}
char gpsDisplayInfo()
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
    Serial.println("Location: Not Available");
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
  delay(1000);
  char data = gpsSerial.read(); 
  return  data;
}

float temp(){
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  return temperatureC;
}
void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.println("Connection failed, retrying in 5 seconds...");
      delay(5000);
    }
  }
}

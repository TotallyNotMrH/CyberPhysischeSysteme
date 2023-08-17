#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <ThingSpeak.h>

#define DHT_PIN 4
#define ONE_WIRE_BUS 16
#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);
LiquidCrystal_I2C lcd (0x27, 16,2);  

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// für den jeweiligen DS1820 Sensor anpassen oder den OneWire-Bus mittels Schleife durchlaufen
DeviceAddress sensor1 = { 0x28, 0x64, 0x39, 0x77, 0x91, 0x13, 0x2, 0xDE };

// WLAN-Credentials
const char* ssid = "...";
const char* password = "...";

WiFiClient client;

// auf den jew. ThingSpeak-Kanal anpassen
unsigned long tsChannelNumber = ...;
const char* tsWriteAPIKey = "...";


void setup(void){
  Serial.begin(115200);
  dht.begin(); // init DHT sensor
  sensors.begin(); // init DS18B20 sensors on OneWire bus
  lcd.init(); // init lcd1602 on I2C bus
  lcd.backlight(); // backlight on

  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to " + String(ssid));
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    WiFi.begin(ssid, password); 
    delay(5000);     
   } 
   Serial.println("\nConnected.");  
   
   ThingSpeak.begin(client);
}

void loop(void){ 
  
  // lcd.clear();
   
  sensors.requestTemperatures(); // Send the command to get temperatures
  String tmpDS = String(sensors.getTempC(sensor1));
  Serial.print("Sensor 1(*C): ");
  Serial.println(tmpDS); 
  lcd.setCursor(0,0);
  lcd.print("DS: " + tmpDS);
    
  
  float tmp = dht.readTemperature();
  String tmpDHT = "0";
  if (!isnan(tmp)) {
    tmpDHT = tmp;
  }

  Serial.print("Sensor 2(*C): ");
  Serial.println(tmpDHT);
  lcd.setCursor(0,1);
  lcd.print("DHT: " + tmpDHT + "        ");
  
  ThingSpeak.setField(1, tmpDS);
  ThingSpeak.setField(2, tmpDHT);
  int ret = ThingSpeak.writeFields(tsChannelNumber, tsWriteAPIKey);
   if(ret == 200){
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(ret));
  } 
  
  delay(60000);
}

#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <ThingSpeak.h>

#define DHT_PIN 4
#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);
LiquidCrystal_I2C lcd (0x27, 16,2);  

// WLAN-Credentials
const char* ssid = "CPSLABOR2";
const char* password = "1234567890";


unsigned long tsChannelNumber = ...;
const char* tsWriteAPIKey = "...";

WiFiClient client;
ThingSpeak.begin(client);


void setup(void){
  Serial.begin(115200);
  dht.begin(); // init DHT sensor
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
}

void loop(void){ 
  // lcd.clear();

  float tmp = dht.readTemperature();
  String tmpDHT = "0";
  if (!isnan(tmp)) {
    tmpDHT = tmp;
  }
  Serial.print("Temperatur(*C): ");
  Serial.println(tmpDHT);
  lcd.setCursor(0,0);
  lcd.print("Temperatur: " + tmpDHT + "        ");

  float hum = dht.readHumidity();
  String humDHT = "0";
  if (!isnan(hum)) {
    humDHT = hum;
  }
  Serial.print("Luftfeuchtigkeit(%): ");
  Serial.println(humDHT);
  lcd.setCursor(0,1);
  lcd.print("Luftfeuchtigkeit: " + humDHT + "        ");
  ThingSpeak.setField(1, tmp);
  ThingSpeak.setField(2, hum);
  
  delay(10000);
}

#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <ThingSpeak.h>
//hier muss eine Bibliothek für MQTT eingebunden werden

#define DHT_PIN 12
#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);
LiquidCrystal_I2C lcd (0x27, 16,2);  

// WLAN-Zugansdaten
const char* ssid = "AUSFUELLEN";
const char* password = "AUSFUELLEN";

WiFiClient client;
// PubSubClient erzeugen

//Thingspeak-Daten: MQTT-Server/Port sowie die exportierten MQTT-Daten
const char* MQTT_SERVER = "mqtt3.thingspeak.com";
const long MQTT_PORT = 1883;


const long THINGSPEAK_CHANNEL = ...;
const char MQTT_USERNAME[] = "...";            
const char MQTT_PASSWORD[] = "...";         
const char MQTT_CLIENTID[] = "...";


void setup(void){
  Serial.begin(115200);
  dht.begin();              // init DHT sensor
  lcd.init();               // init lcd1602 on I2C bus
  lcd.backlight();          // backlight on
  connectWiFi();

  // init MQTT client
  // ...
  
}

//hier wird wird der ESP mit dem WLAN verbunden
void connectWiFi(){
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to " + String(ssid));
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    WiFi.begin(ssid, password); 
    delay(5000);     
  } 
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(void){ 
  lcd.clear();

  float hmd = dht.readHumidity();
  String hmdDHT = "0";
  lcd.setCursor(0,0);
  lcd.print("Luftfeuchte: " + hmdDHT);
  
  //Temperatur des DHT11 auslesen
  float tmp = dht.readTemperature();
  String tmpDHT = "0";
  if (!isnan(tmp)) {
    tmpDHT = tmp;
  }
  lcd.setCursor(0,1);
  lcd.print("DHT: " + tmpDHT + "        ");

  delay(4000);
}

void mqttConnect(){
  /*hier soll sich der ESP mit dem Thingspeak-MQTT Broker verbinden.
  bitte hier eine Schleife verwenden, damit eine Verbindung garantiert wird*/
}

void reconnect(){
  /*Hier soll sich der Client neu verbinden*/
}

void mqttSubscribe(long subChannelID){
  /*diese Funktion soll den übergebenen Channel abonieren*/
}

void mqttPublish(long pubChannelID, String message) {
  /*bei Aufruf dieser Funktion soll im übergebenen Channel eine Nachricht veröffentlicht werden
  Diese Nachricht soll verschiendene Felder beinhalten (Die beiden Temperaturen und vll die Luftfeuchtigkeit)*/
}

void mqttSubscriptionCallback( char* topic, byte* payload, unsigned int length ) {
  /*Diese Funktion wird aufgerufen, wenn ein abonnierter Channel geupdated wurde
  /Die empfangene Nachricht (payload) und das Topic sollen auf dem seriellen Monitor ausgegeben werden*/
}

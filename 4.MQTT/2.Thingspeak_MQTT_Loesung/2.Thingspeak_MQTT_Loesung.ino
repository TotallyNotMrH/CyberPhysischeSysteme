#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <ThingSpeak.h>
#include <PubSubClient.h>                 //hier muss eine Bibliothek für MQTT eingebunden werden

#define DHT_PIN 12
#define ONE_WIRE_BUS 16
#define DHTTYPE DHT11

// uncomment to enable LCD-Display
// #define LCD_DISPLAY

// uncomment to subscribe MQTT-Channel (to get a message in Serial via subscription after publishing to a topic)
// #define MQTT_SUBSCRIBE

DHT dht(DHT_PIN, DHTTYPE);

#ifdef LCD_DISPLAY
  #include <LiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd (0x27, 16,2);  
#endif

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// WLAN-Zugansdaten
const char* ssid = "AUSFUELLEN";
const char* password = "AUSFUELLEN";

WiFiClient wifiClient;
PubSubClient client(wifiClient);           //MQTT-Client erzeugen

const char* MQTT_SERVER = "mqtt3.thingspeak.com";
const long MQTT_PORT = 1883;

const long THINGSPEAK_CHANNEL = 490422;
const char MQTT_USERNAME[] = "AUSFUELLEN";            
const char MQTT_PASSWORD[] = "AUSFUELLEN";         
const char MQTT_CLIENTID[] = "AUSFUELLEN";


//globale Variablen zur Zeitmessung
unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 20L * 1000L; // Post data every 20 seconds. 
long lastPublishMillis = 0;
int connectionDelay = 1;
int updateInterval = 15;

void setup(void){
  Serial.begin(115200);
  dht.begin();              // init DHT sensor
  sensors.begin();          // init DS18B20 sensors on OneWire bus
#ifdef LCD_DISPLAY
  lcd.init();               // init lcd1602 on I2C bus
  lcd.backlight();          // backlight on
#endif
  connectWiFi();

  //MQTT-Client wird initialisiert
  client.setServer( MQTT_SERVER, MQTT_PORT );
  client.setCallback( mqttSubscriptionCallback );
  client.setBufferSize( 2048 );

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

  //lcd.clear();
  
  // Temperatur aus dem BUS auslesen
  sensors.requestTemperatures();
  float tmpD = sensors.getTempCByIndex(0);                  
  String tmpDS = String(tmpD);
  Serial.print("Sensor DS (*C): ");
  Serial.println(tmpDS); 

#ifdef LCD_DISPLAY
  lcd.setCursor(0,0);
  lcd.print("DS: " + tmpDS);
#endif
  
  //Temperatur des DHT11 auslesen
  float tmp = dht.readTemperature();
  String tmpDHT = "0";
  if (!isnan(tmp)) {
    tmpDHT = tmp;
  }
  Serial.print("Sensor DHT (*C): ");
  Serial.println(tmpDHT);
#ifdef LCD_DISPLAY
  lcd.setCursor(0,1);
  lcd.print("DHT: " + tmpDHT + "        ");
#endif

  //verbinden, falls noch nicht geschehen
  if (WiFi.status() != WL_CONNECTED) {
      connectWiFi();
  }
  if (!client.connected()) {
     mqttConnect(); 
#ifdef MQTT_SUBSCRIBE     
     mqttSubscribe(THINGSPEAK_CHANNEL);
#endif
  }
  client.loop(); 
  // Update ThingSpeak channel periodically. The update results in the message to the subscriber.
  mqttPublish(THINGSPEAK_CHANNEL, (String("field1=" + String(tmp) + "&field2=" + String(tmpD) )));
  
  delay(4000);
}

void mqttConnect(){
  /*hier soll sich der ESP mit dem Thingspeak-MQTT Broker verbinden.
  bitte hier eine Schleife verwenden, damit eine Verbindung garantiert wird*/
  while ( !client.connected() ){
    // Connect to the MQTT broker.
    if ( client.connect( MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD ) ) {
      Serial.print( "MQTT to " );
      Serial.print( MQTT_SERVER );
      Serial.print (" at port ");
      Serial.print( MQTT_PORT );
      Serial.println( " successful." );
    } else {
      Serial.print( "MQTT connection failed, rc = " );
      Serial.print( client.state() );
      Serial.println( " Will try again in a few seconds" );
      delay( connectionDelay*1000 );
    }
  }
}

void reconnect(){
  /*Hier soll sich der Client neu verbinden*/
  Serial.println("Attempting MQTT connection");
  if (client.connect(MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD))  {
    Serial.println("Conn:"+ String(MQTT_SERVER) + " cl: " + String(MQTT_CLIENTID)+ " Uname:" + String(MQTT_USERNAME));
  } else {
    Serial.println("Failed to connect. Trying to reconnect in 2 seconds");
    delay(2000);
  } 
}

void mqttSubscribe(long subChannelID){
  /*diese Funktion soll den übergebenen Channel abonieren*/
  String myTopic = "channels/"+String( subChannelID )+"/subscribe";
  client.subscribe(myTopic.c_str());
}

void mqttPublish(long pubChannelID, String message) {
  /*bei Aufruf dieser Funktion soll im übergebenen Channel eine Nachricht veröffentlicht werden
  Diese Nachricht soll verschiendene Felder beinhalten (Die beiden Temperaturen und vll die Luftfeuchtigkeit)*/
  String topicString ="channels/" + String( pubChannelID ) + "/publish";
  client.publish( topicString.c_str(), message.c_str() );
}

void mqttSubscriptionCallback( char* topic, byte* payload, unsigned int length ) {
  /*Diese Funktion wird aufgerufen, wenn ein abonnierter Channel geupdated wurde
  /Die empfangene Nachricht (payload) und das Topic sollen auf dem seriellen Monitor ausgegeben werden*/
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "Home_T";
const char* password = "HW111213";

#define PIN_LED_1 26
#define PIN_LED_2 27
 
WebServer server(443);
int curStatus1 = LOW;
int curStatus2 = LOW;

 
void allLedOn() {
  led1on();
  led2on();
}

void allLedOff() {
  led1off();
  led2off();
}

void led1on() {

  curStatus1 = HIGH;
  digitalWrite(PIN_LED_1, curStatus1);
  led1status();
  
  
}

void led1off() {

  curStatus1 = LOW;
  digitalWrite(PIN_LED_1, curStatus1);
  led1status();
  
}

void led1status() {
    server.send(200, "application/json", String(curStatus1));
}

 
void led2on() {

  curStatus2 = HIGH;
  digitalWrite(PIN_LED_2, curStatus2);
  led2status();
}

void led2off() {

  curStatus2 = LOW;
  digitalWrite(PIN_LED_2, curStatus2);
  led2status();
  
}

void led2status() {
  server.send(200, "application/json", String(curStatus2));
}

void allStatus() {
  
  StaticJsonDocument<300> JSONdoc; 
  
  byte i = 0;
  
  JSONdoc["LED"][i]["id"] = 1;
  JSONdoc["LED"][i]["pin"] = PIN_LED_1;
  JSONdoc["LED"][i]["status"] = curStatus1;

  i++;
  JSONdoc["LED"][i]["id"] = 2;
  JSONdoc["LED"][i]["pin"] = PIN_LED_2;
  JSONdoc["LED"][i]["status"] = curStatus2;
  
  String JSONmessageBuffer;
  serializeJson(JSONdoc, JSONmessageBuffer);
  server.send(200, "application/json", JSONmessageBuffer);
}



// Manage not found URL
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
 
void setup(void) {
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

 
  // bei HTTP-Aufruf der entsprechenden URL Ausführen der Funktion ledXon/ledXoff
  server.on(F("/LED/"), HTTP_GET, allStatus); 
  server.on(F("/LED/on"), HTTP_GET, allLedOn); 
  server.on(F("/LED/off"), HTTP_GET, allLedOff); 
  server.on(F("/LED/1/"), HTTP_GET, led1status); 
  server.on(F("/LED/1/on"), HTTP_GET, led1on); 
  server.on(F("/LED/1/off"), HTTP_GET, led1off); 
  server.on(F("/LED/2/"), HTTP_GET, led2status); 
  server.on(F("/LED/2/on"), HTTP_GET, led2on); 
  server.on(F("/LED/2/off"), HTTP_GET, led2off); 
  
  // falls URL nicht gefunden, Fehlebehandlung durch die Funktion handleNotFound
  server.onNotFound(handleNotFound);
  
  // start server
  server.begin();
  
  Serial.println("HTTP server started");

  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  digitalWrite(PIN_LED_1, LOW);
  digitalWrite(PIN_LED_2, LOW);

}
 
void loop(void) {
  server.handleClient();
}

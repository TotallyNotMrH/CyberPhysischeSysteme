#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

const char* ssid = "...";
const char* password = "...";

#define PIN_LED 13
 
WebServer server(80);
int pastStatus = LOW;

void toggleLED() {

  int newStatus = LOW;
  
  if (pastStatus == LOW) {
    newStatus = HIGH;
  }
  pastStatus = newStatus;
  
  digitalWrite(PIN_LED, newStatus);

  server.send(200, "application/json", String(newStatus));
  
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
 
  // bei HTTP-Aufruf der URL /toggleLED Ausführen der Funktion toggleLED
  server.on(F("/toggleLED"), HTTP_GET, toggleLED); 
  
  // falls URL nicht gefunden, Fehlebehandlung durch die Funktion handleNotFound
  server.onNotFound(handleNotFound);
  
  // start server
  server.begin();
  
  Serial.println("HTTP server started");

  pinMode(PIN_LED, OUTPUT);

}
 
void loop(void) {
  server.handleClient();
}

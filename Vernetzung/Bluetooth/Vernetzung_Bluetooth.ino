#include "BluetoothSerial.h"
#include "DHT.h"

#define DHTPIN  4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("HechtESP"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  SerialBT.println("Temperatur: "+String(temperature).c_str());
  delay(2000);
  SerialBT.println(("Luftfeuchtigkeit: "+String(humidity)).c_str());
  if (Serial.available()) {
    SerialBT.println("Ich bin noch da");
  }
  if (SerialBT.available()) {
    
    Serial.write(SerialBT.read());
  }
  delay(2000);
}
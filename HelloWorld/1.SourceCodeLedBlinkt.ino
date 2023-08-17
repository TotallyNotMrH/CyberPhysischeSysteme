/* Die Define-Anweisung wird vor dem Kompilieren des Programms ausgeführt; sie    ersetzt alle Vorkommnisse von »PIN_LED« im Code mit der Zahl 13. */  
#define PIN_LED 13

// Die SETUP-Funktion wird einmalig beim Start des Microcontrollers ausgeführt
void setup() {
    
    // definiert den Zweck eine PINs, Eingang (INPUT) oder Ausgang (OUTPUT)
    pinMode(PIN_LED, OUTPUT);
}

// Die LOOP-Funktion läuft als Endlosschleife auf dem Microcontroller
void loop() {
  
  // versetzt einen PIN in einen elektrischen Zustand, hier »Strom Ein«
  digitalWrite(PIN_LED, HIGH);
  // 1000 ms Verzögerung/Pause; 
  delay(1000);

  // »Strom Aus«
  digitalWrite(PIN_LED, LOW);
  // wieder Pause
  delay(1000);
}

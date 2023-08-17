#define PIN_INTERNAL 2
#define PIN_LED 13
#define PIN_REED 15

void setup() {
  
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_INTERNAL, OUTPUT);
  pinMode(PIN_REED, INPUT_PULLUP);
}

void loop() {

  int rState = digitalRead(PIN_REED);
  int myState = HIGH;
  if (rState) {
   myState = LOW;
  }
  digitalWrite(PIN_LED, myState);
  digitalWrite(PIN_INTERNAL, rState);
  
}

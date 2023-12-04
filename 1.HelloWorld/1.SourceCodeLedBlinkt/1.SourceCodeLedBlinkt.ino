#define PINLED 22
#define PINSENSOR 5
//HalloWelt
void setup() {
  
  pinMode(PINSENSOR, INPUT);  
  Serial.begin(9600);
}

void loop() {
  bewegungsstatus = digitalRead(bewegung);
  


}
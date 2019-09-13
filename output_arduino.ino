#define dataPin 11
#define clockPin 12

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  byte message = 200;

  for (int i = 0; i < 8; i++) {
    digitalWrite(clockPin, LOW);
    delay(100);
    digitalWrite(dataPin, message % 2);
    delay(100);
    digitalWrite(clockPin, HIGH);
    delay(200);
    message = message>>1;
  }

  Serial.println("Done cycle");

}

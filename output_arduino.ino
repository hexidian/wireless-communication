#define dataPin 11
#define clockPin 12
#define tickLength 400

#define recievePin 8

void (* resetFunc) (void) = 0;

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(recievePin, INPUT);
  Serial.begin(9600);
}

void loop() {

  byte message = 100;

  for (int i = 0; i < 8; i++) {
    digitalWrite(clockPin, LOW);
    delay(tickLength / 2);
    digitalWrite(dataPin, message % 2);
    delay(tickLength / 2);
    digitalWrite(clockPin, HIGH);
    delay(tickLength / 2);
    digitalWrite(clockPin, LOW);
    message = message >> 1;
  }

  checkCorrectSent();

  Serial.println("Done cycle");

}

void checkCorrectSent() {
  delay(tickLength / 2);
  digitalWrite(clockPin, HIGH);
  delay(tickLength / 2);
  if (digitalRead(recievePin)) {
    return;
  } else {
    delay(tickLength/2);
    digitalWrite(clockPin, LOW);
    resetFunc();
  }
}

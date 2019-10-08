#define dataPin 11
#define clockPin 12
#define tickLength 400

#define recievePin 8
#define authorizeConfirmationPin 7

void (* resetFunc) (void) = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("resetting");
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(recievePin, INPUT);
  pinMode(authorizeConfirmationPin, OUTPUT);
  digitalWrite(authorizeConfirmationPin, LOW);
}

void loop() {

  byte message = 100;

  for (int i = 0; i < 8; i++) {
    digitalWrite(clockPin, LOW);
    delay(tickLength / 2);
    digitalWrite(dataPin, message % 2);
    Serial.print("sending ");
    Serial.println(message % 2);
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
  digitalWrite(clockPin, LOW);//just make sure
  digitalWrite(authorizeConfirmationPin, HIGH);

  delay(tickLength/2);

  while (!digitalRead(recievePin)) {//wait until we receive the first high signal

    //the other one might be off by a bit, so we have to pulse the clock

    digitalWrite(clockPin, HIGH);
    delay(tickLength/2);
    digitalWrite(clockPin, LOW);
    delay(tickLength/2);
    
  }

  Serial.println("first pulse received");

  delay(tickLength / 2);
  digitalWrite(authorizeConfirmationPin, LOW);
  delay(tickLength / 2);
  digitalWrite(authorizeConfirmationPin, HIGH);
  delay(tickLength);

  //now we check if there will be a second signal
  Serial.print("receiving: "); Serial.println(digitalRead(recievePin));
  delay(20);//to fix serial error
  if (digitalRead(recievePin)) {
    Serial.println("second pulse received");
    digitalWrite(authorizeConfirmationPin, LOW);
    Serial.println("Correct Message Confirmed");
    return;
  } else {
    //Serial.println("Incorrect Message Sent");
    digitalWrite(authorizeConfirmationPin, LOW);
    delay(20);//make sure it happens
    resetFunc();
  }
}

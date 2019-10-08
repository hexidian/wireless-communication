#define clock_pin 7
#define data_pin 8

#define error_pin 4
#define ready_confirmation_pin 3

byte message;

int bits;
bool first_bit;
void setup() {

  pinMode(clock_pin, INPUT);
  pinMode(data_pin, INPUT);

  pinMode(error_pin, OUTPUT);
  pinMode(ready_confirmation_pin, INPUT);

  message = 0;
  bits = 0;
  first_bit = 1;

  Serial.begin(9600);
}

void loop() {

  if (digitalRead(clock_pin)) {
    if (first_bit) {
      first_bit = 0;
      while (digitalRead(clock_pin)) {
        delay(10);//wait until clock cycle is over
      }
    } else {
      bool this_bit = 1 ? digitalRead(data_pin) : 0;
      Serial.print("this bit is ");
      Serial.println(this_bit);
      message = message >> 1;
      message += this_bit * 0b10000000;
      bits++;

      while (digitalRead(clock_pin)) {
        delay(10);//wait for clock cycle to end
      }

    }
  }

  if (bits == 8) {
    bits = 0;
    if (checkMessage(message)) {
      Serial.println(message);
      message = 0;
      transmitSuccess();
    } else {
      message = 0;
      Serial.println("Error detected");
      transmitError();
    }
  }

}

bool checkMessage(byte message) {
  if (message == 100) {
    return 1;
  }
  return 0;
}

void transmitSuccess() {
  while (!digitalRead(ready_confirmation_pin)) {
    delay(2);//wait for pulse
  }
  digitalWrite(error_pin, HIGH);//the tell other one we are ready
  Serial.println("first pulse");
  while (digitalRead(ready_confirmation_pin)) {
    delay(2);//wait for clock pulse to end
  }

  digitalWrite(error_pin, LOW);

  while (!digitalRead(ready_confirmation_pin)) {
    delay(2);//wait for clock pulse to come back
  }

  digitalWrite(error_pin, HIGH);
  Serial.println("second pulse");

  while (digitalRead(ready_confirmation_pin)) {
    delay(2);//wait for clock pulse to end
  }

  digitalWrite(ready_confirmation_pin, LOW);


  return;
}

void transmitError() {
  while (!digitalRead(ready_confirmation_pin)) {
    delay(2);//wait for pulse
  }
  digitalWrite(error_pin, HIGH);//the tell other one we are ready
  while (digitalRead(ready_confirmation_pin)) {
    delay(2);//wait for clock pulse to end
  }

  digitalWrite(error_pin, LOW);

  while (!digitalRead(ready_confirmation_pin)) {
    delay(2);//wait for clock pulse to come back
  }

  while (digitalRead(ready_confirmation_pin)) {
    delay(2);//wait for clock pulse to end
  }

  return;
}

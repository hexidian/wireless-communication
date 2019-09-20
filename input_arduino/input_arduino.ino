#define clock_pin 7
#define data_pin 8

#define error_pin 4

byte message;

int bits;
bool first_bit;
void setup() {

  pinMode(clock_pin, INPUT);
  pinMode(data_pin, INPUT);

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
      message *= 2;
      message += this_bit;
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
      Serial.println("Error detected");
      transmitError();
    }
  }

}

bool checkMessage(byte message) {
  Serial.println(message);
  if (message == 100) {
    return 1;
  }
  return 0;
}

void transmitSuccess() {
  while (!digitalRead(clock_pin)) {
    delay(10);//wait for clock pulse
  }
  digitalWrite(error_pin, HIGH);
  while (digitalRead(clock_pin)) {
    delay(10);//wait for clock pulse to end
  }
  return;
}

void transmitError() {
  first_bit = 1;//because other arduino will be reseting

  while (!digitalRead(clock_pin)) {
    delay(10);//wait for clock pulse
  }
  while (digitalRead(clock_pin)) {
    delay(10);//wait for clock pulse to end
  }
  return;
}

/* The example code is taken from 
https://docs.arduino.cc/language-reference/en/functions/external-interrupts/digitalPinToInterrupt/*/

// Try 33 or more to get 'not a valid interrupt pin' message
int pin = 2;

void setup() {
  Serial.begin(115200);
  int checkPin = digitalPinToInterrupt(pin);

  if (checkPin == -1) {
    Serial.println("Not a valid interrupt pin!");
  } else {
    Serial.println("Valid interrupt pin.");
  }
}

void loop() {
}
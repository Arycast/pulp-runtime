/* This example is taken from https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalwrite/ */
/* LED2 is blinking controlled by the written value */

void setup() {
  pinMode(LED2, OUTPUT);    // sets the digital LED2 as output
}

void loop() {
  digitalWrite(LED2, HIGH); // sets the digital LED2 on
  delay(1000);            // waits for a second
  digitalWrite(LED2, LOW);  // sets the digital LED2 off
  delay(1000);            // waits for a second
}
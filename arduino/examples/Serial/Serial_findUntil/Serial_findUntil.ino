/*
  Serial Communication API
  : Serial.findUntil() function

  Reads data from the serial buffer until a target string of given length or terminator string is found
  This function inherits from Stream class

  This example code waits for the target string OK to appear before the terminator CANCEL
*/

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Type a message. Looking for 'OK', but will stop if 'CANCEL' appears.");
}

void loop() {
  if (Serial.available()) {
    /* Try to find "OK", but stop early if "CANCEL" is found first */
    if (Serial.findUntil("OK", "CANCEL")) {
      Serial.println("Found OK before CANCEL!");
    } else {
      Serial.println("Did not find OK (CANCEL appeared first or timed out).");
    }
  }

  delay(1000);
}
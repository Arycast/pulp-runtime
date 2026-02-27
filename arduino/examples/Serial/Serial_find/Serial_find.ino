/*
  Serial Communication API
  : Serial.find() function

  This function reads data from the serial buffer until the target is found
  Serial.find() inherits from Stream class

  This example code waits for the target string OK to appear

  Example :
  if OK is sent from serial monitor, "Received OK!" will appear in serial monitor
  else "Didn't find OK, try again." will appear in serial monitor

  Notes :
  Make sure to use 115200 baudrate
*/

void setup() {
  Serial.begin(115200);

  Serial.println("Type 'OK' to continue:");
}

void loop() {
  if (Serial.available()) {
    /* Look for the word "OK" in the incoming serial data */
    if (Serial.find("OK")) {
      Serial.println("Received OK!");
    } else {
      Serial.println("Didn't find OK, try again.");
    }
  }

  delay(1000);
}
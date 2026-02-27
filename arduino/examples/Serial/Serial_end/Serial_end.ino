/*
  Serial Communication API
  : Serial.end() function

  Disables serial communication, allowing the RX and TX pins to be used for general input and output
  To re-enable serial communication, call Serial.begin()

  This example code disables and then enables serial communication
*/

void setup() {
  Serial.begin(115200);
  delay(1000); /* Allow time for Serial Monitor to connect */
  Serial.println("Hello from Serial!");

  Serial.end();  /* Stop communication */

  delay(2000);   /* Simulate doing something else... */

  Serial.begin(115200);  /* Re-initialize serial */
  Serial.println("Serial restarted!");
}

void loop() {
}
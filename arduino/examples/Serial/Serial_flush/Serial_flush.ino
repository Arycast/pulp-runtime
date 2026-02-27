/*
  Serial Communication API
  : Serial.flush() function

  Waits for the transmission of outgoing serial data to complete
  This function inherits from the Stream class

  This example code waits until the whole serial buffer is sent, then turns on an LED
*/

void setup() {
  Serial.begin(115200);
  
  Serial.print("Sending... "); /* this could be a large amount of data */
  Serial.flush();  /* Waits until everything above is actually sent */
  digitalWrite(LED_BUILTIN, HIGH); /* Only turns LED on after data is sent */
}

void loop() {
}
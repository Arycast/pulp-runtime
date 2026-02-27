/*
  Serial Communication API
  : Serial.readBytesUntil() function

  Reads characters from the serial buffer into an array
  The function terminates if the determined length has been read, if it times out, or if the terminator character is detected
  The function returns the characters up to the last character before the supplied terminator
  The terminator itself is not returned in the buffer

  This function inherits from Stream class

  This example code reads until a given character is found and stores the data in a buffer to then print it
*/

char message[20];  /* Buffer to hold the message */

void setup() {
  Serial.begin(115200);

  Serial.println("Type a message (end with Enter):");
}

void loop() {
  if (Serial.available()) {
    delay(10);
    int bytesRead = Serial.readBytesUntil('\n', message, sizeof(message) - 1);
    message[bytesRead] = '\0';  /* Null-terminate the string */

    Serial.print("You said: ");
    Serial.println(message);
  }
}
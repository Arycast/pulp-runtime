/*
  Serial Communication API
  : Serial.availableForWrite() function

  Get the number of bytes (characters) available for writing in the serial buffer without blocking the write operation

  This example code checks if there is enough space in the serial buffer to write

  Notes and Warnings :
  If using a software-implemented UART, the function will return 0. It will only return the available bytes in HardwareSerial
*/

void setup() {
  Serial.begin(115200); /* Start serial communication */
}

void loop() {
  int bytesAvailable = Serial.availableForWrite(); /* Check how many bytes we can write */

  Serial.print("Bytes available to write: ");
  Serial.println(bytesAvailable);

  if (bytesAvailable > 13) {
    Serial.println("Hello, world!"); /* Send a message only if enough buffer space is available */
  }

  delay(1000); /* Wait a second before checking again */
}
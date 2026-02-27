/*
  Serial Communication API
  : Serial.write() function

  Writes binary data to the serial port
  This data is sent as a byte or series of bytes
  to send the characters representing the digits of a number use the print() function instead

  This example code send the string "hello" and return the length of the string

  Notes and Warnings :
  Serial transmission is asynchronous
  If there is enough empty space in the transmit buffer, Serial.write()will return before any characters are transmitted over serial
  If the transmit buffer is full then Serial.write() will block until there is enough space in the buffer
  To avoid blocking calls to Serial.write(), first check the amount of free space in the transmit buffer using availableForWrite()
*/

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.write(45); /* send a byte with the value 45 */

  int bytesSent = Serial.write("hello");  /* send the string "hello" and return the length of the string */
}
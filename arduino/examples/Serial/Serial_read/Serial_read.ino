/*
  Serial Communication API
  : Serial.read() function

  Reads incoming serial data and consumes it from the serial buffer
  This function inherits from Stream class

  This example code will print the decimal value of Serial input value and convert it into character
*/

int incomingByte = 0; /* for incoming serial data */

void setup() {
  Serial.begin(115200); /* opens serial port */
}

void loop() {
  /* send data only when you receive data */
  if (Serial.available() > 0) {
    /* read the incoming byte: */
    incomingByte = Serial.read();

    /* say what you got: */
    Serial.print("I received: ");
    Serial.print(incomingByte, DEC);

    /* convert into character*/
    Serial.print(" | ");
    Serial.println((char) incomingByte);
  }
}
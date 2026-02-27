/*
  Serial Commmunication API
  : Serial.available() function

  Get the number of bytes (characters) available for reading from the serial port
  This refers to data that has already been received and is currently stored in the serial receive buffer (which holds 64 bytes)

  This function inherits from the Stream class

  This example code returns a character received through the serial port and convert it into readable character
  This code also print the return of Serial.available()

  Note :
  Add small delay after if condition of Serial.available, it can wait all the serial input enter the buffer first
*/

int incomingByte = 0; /* for incoming serial data */
int avail = 0; /* for Serial.available() return data */

void setup() {
  Serial.begin(115200); /* begin the serial*/
}

void loop() {
  /* reply only when receive data: */
  if (Serial.available() > 0) {
    delay(10); /* small delay to wait all serial data enter the buffer */

    /* read the amount of entering data */
    avail = Serial.available();

    /* print the amount of entering data */
    Serial.print("amount of entering data : ");
    Serial.println(avail);

    /* read the incoming byte: */
    incomingByte = Serial.read();

    /* print the incomingByte */
    Serial.print("I received: ");
    Serial.print(incomingByte, DEC);
    Serial.print (" | ");
    Serial.println((char) incomingByte); /* convert incomingByte into character */

  }

}
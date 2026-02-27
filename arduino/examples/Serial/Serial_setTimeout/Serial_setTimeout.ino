/*
  Serial Communication API
  : Serial.setTimeout() function

  Sets the maximum milliseconds to wait for serial data
  It defaults to 1000 milliseconds

  This example code modifies the default serial timeout time

  Notes and Warnings :
    Serial functions that use the timeout value set via setTimeout() :
      * Serial.find();
      * Serial.findUntil();
      * Serial.parseInt();
      * Serial.parseFloat();
      * Serial.readBytes();
      * Serial.readBytesUntil();
      * Serial.readString();
      * Serial.readStringUntil();
*/

char data[6]; /* 5 characters + null terminator */

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.setTimeout(5000); /* Set timeout to 5 seconds */

  Serial.println("Send 5 characters (you have 5 seconds):");
}

void loop() {
  if (Serial.available()) {
    int bytesRead = Serial.readBytes(data, 5);
    data[bytesRead] = '\0';

    Serial.print("Received: ");
    Serial.println(data);
  }
}
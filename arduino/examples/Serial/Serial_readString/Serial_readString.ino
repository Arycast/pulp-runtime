/*
  Serial Communication API
  : Serial.readString() function

  Reads characters from the serial buffer into a String
  The function terminates if it times out

  This example code will return "A primary color" if String "red" is received

  Notes and Warnings:
  The function does not terminate early if the data contains end of line characters.
  The returned String may contain carriage return and/or line feed characters if they were received.
*/

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("Enter data:");
  while (Serial.available() == 0) {}     /* wait for data available */
  delay(10); /* wait Serial input data to enter the Serial buffer */
  String teststr = Serial.readString();  /* read until timeout */
  if (teststr == "red") {
    Serial.println("A primary color");
  } else {
    Serial.println("Something else");
  }
}
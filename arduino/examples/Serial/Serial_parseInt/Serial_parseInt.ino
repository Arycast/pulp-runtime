/*
  Serial Communication API
  : Serial.parseInt() function

  Looks for the next valid integer in the incoming serial
  Parsing stops when no characters have been read for a configurable time-out value, or a non-digit is read

  This example code extracts the first valid integer number received on the serial

  Example :
  if Serial input value is "The size of the box is 5 mm", it will return 5
  if Serial input value is "The size of the box is -5 mm", it will return -5
*/

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1000);
  Serial.println("Enter a integer number:");  /* try with a mixed phrase e.g. "The size of the box is 5 mm" */
}

void loop() {
  if (Serial.available()) {
    delay(10); /* wait the serial data to enter ther serial buffer*/
    int number = Serial.parseInt();
    if (number != 0) {
      Serial.print("You entered: ");
      Serial.println(number);
    }
  }
}
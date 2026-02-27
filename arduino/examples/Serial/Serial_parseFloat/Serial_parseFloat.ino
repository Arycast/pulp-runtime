/*
  Serial Communication API
  : Serial.parseFloat() function

  Retrieves the first valid floating point number from the Serial buffer
  parseFloat() is terminated by the first character that is not a floating point number
  The function terminates if it times out
  This function can return negative and fraction value

  This function inherits from Stream class

  This example code extracts the first valid floating point number received on the serial

  Example : 
  if Serial input is "The size of the box is 3.5 mm", it will return 3.50
  if Serial input is "The size of the box is -3.5 mm", it will return -3.50
*/

void setup() {
  Serial.begin(115200);

  Serial.println("Enter a floating-point number:");  /* try with a mixed phrase e.g. "The size of the box is 3.5 mm" */
}

void loop() {
  if (Serial.available()) {
    delay(10); /* add small delay to wait serial data entering the buffer*/
    float number = Serial.parseFloat();
    if (number != 0) {
      Serial.print("You entered: ");
      Serial.println(number);
      Serial.read();
    }
  }
}
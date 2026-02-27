/*
  Serial Communication API
  : Serial.peek() function

  Returns the next byte (character) of incoming serial data without removing it from the internal serial buffer
  That is, successive calls to peek() will return the same character, as will the next call to read()
  This function inherits from Stream class

  This example code gives a first look at what is coming through serial and then prints it
*/

void setup() {
  Serial.begin(115200);
  Serial.println("Type something:");
}

void loop() {
  if (Serial.available()) {
    char nextChar = Serial.peek();  /* Look at the next character */
    Serial.print("Next character will be: ");
    Serial.println(nextChar);

    char actualChar = Serial.read();  /* Now actually consume it */
    Serial.print("Read character: ");
    Serial.println(actualChar);
  }
}
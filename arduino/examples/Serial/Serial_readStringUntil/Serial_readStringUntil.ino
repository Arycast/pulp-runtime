/*
  Serial Communication API
  : Serial.readStringUntil() function

  Reads characters from the serial buffer into a String up to terminator character
  The function terminates if it times out 
  This function inherits from Stream class

  This example code reads the serial incoming data until a newline is received

  Notes and Warnings :
  The terminator character is discarded from the serial buffer
  If the terminator character can’t be found, all read characters will be discarded
*/

void setup() {
  Serial.begin(115200);

  Serial.println("Enter a word and press Enter:");
}

void loop() {
  if (Serial.available()) {
    delay(10); /* wait Serial incoming data to enter the Serial buffer */
    String input = Serial.readStringUntil('\n');  /* Read until newline */
    Serial.print("You entered: ");
    Serial.println(input);
  }
}
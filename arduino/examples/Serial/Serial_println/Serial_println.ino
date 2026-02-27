/*
  Serial Communication API
  : Serial.println() function

  Prints data to the serial port as human-readable ASCII text followed by a carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n').
  This command takes the same forms as Serial.print()

  This example code print ASCII-encoded decimal value in various format
*/

int Value = 65;    /* ASCII character code for "A" (try with 71 for "G" and 97 for "a") */

void setup() {
  Serial.begin(115200);
}

void loop() {

  /* print it out in many formats: */
  Serial.println((char)Value);       /* print as an character */
  Serial.println(Value, DEC);  /* print as an ASCII-encoded decimal */
  Serial.println(Value, HEX);  /* print as an ASCII-encoded hexadecimal */
  Serial.println(Value, OCT);  /* print as an ASCII-encoded octal */
  Serial.println(Value, BIN);  /* print as an ASCII-encoded binary */
}
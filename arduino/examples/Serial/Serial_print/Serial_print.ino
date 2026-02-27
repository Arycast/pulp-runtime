/*
  Serial Communication API
  : Serial.print() function

  Prints data to the serial port as human-readable ASCII text
  This command can take many forms:
    * Numbers are printed using an ASCII character for each digit
    * Floats are printed similarly to ASCII digits, defaulting to two decimal places
    * Bytes are sent as a single character. Characters and strings are sent as is

  An optional second parameter specifies the base (format) to use
  
  This example code print numbers in various formats
*/
void setup() {
  Serial.begin(115200);
}

void loop() {
  // print labels
  Serial.print("NO FORMAT");  /* prints a label */
  Serial.print("\t");         /* prints a tab */

  Serial.print("DEC");
  Serial.print("\t");

  Serial.print("HEX");
  Serial.print("\t");

  Serial.print("OCT");
  Serial.print("\t");

  Serial.print("BIN");
  Serial.println();           /* carriage return after the last label */

  for (int x = 0; x < 64; x++) { /* only part of the ASCII chart, change to suit */
    /* print it out in many formats: */
    Serial.print(x);          /* print as an ASCII-encoded decimal - same as "DEC" */
    Serial.print("\t\t");     /* prints two tabs to accomodate the label length */

    Serial.print(x, DEC);     /* print as an ASCII-encoded decimal */
    Serial.print("\t");       /* prints a tab */

    Serial.print(x, HEX);     /* print as an ASCII-encoded hexadecimal */
    Serial.print("\t");       /* prints a tab */

    Serial.print(x, OCT);     /* print as an ASCII-encoded octal */
    Serial.print("\t");       /* prints a tab */

    Serial.println(x, BIN);   /* print as an ASCII-encoded binary */
    /* then adds the carriage return with "println" */
    delay(200);               /* delay 200 milliseconds */
  }
  Serial.println();           /* prints another carriage return */
}
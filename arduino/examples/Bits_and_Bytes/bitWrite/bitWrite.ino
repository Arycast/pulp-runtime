/*
  Bits and Bytes API
  : bitWrite() function

  Writes to a bit of a variable, e.g. bool, int, long
  Note that float & double are not supported

  This code demonstrates the use of bitWrite by printing the value of a variable to the Serial Monitor
  Before and after bitWrite function

  Example :
  * bitWrite(x, n, bit)
  if n = 0; bit = 1; It will return x = 10000001
  if n = 7; bit = 0; it will return x = 00000000
  if n = 2; bit = 1; it will return x = 10000100
*/

void setup() {
  Serial.begin(115200);
  byte x = 0b10000000;  /* the 0b prefix indicates a binary constant */
  Serial.println(x, BIN); /* 10000000 */
  bitWrite(x, 2, 1);  /* write 1 to the least significant bit of x */
  Serial.println(x, BIN); /* 10000001 */
}

void loop() {}
/*
  Bits and Bytes API
  : highByte() function

  Extracts the high (most significant) 8 bits from a 16-bit integer (int, unsigned int, etc.)
  Useful when dealing with communication protocols (like I2C, SPI, serial) where data must be sent in bytes, or when manually splitting numbers

  This code return the "high" (most significant) byte (8-bits) from 16-bit integer

  Example :
  if x = 0xABCD; it will return AB
  if x = 0xCDAB; it will return CD
  if x = 0x85A6; it will return 85
*/

unsigned int x = 0x85A6; /* hex variable */

void setup() {
  Serial.begin(115200);
  
  byte high = highByte(x);  /* Extracts 0xAB */

  Serial.print("The high byte is: ");
  Serial.println(high, HEX);  /* Prints "AB" */
 
}

void loop() {
}
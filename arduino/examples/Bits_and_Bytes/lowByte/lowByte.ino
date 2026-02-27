/*
  Bits and Bytes API
  : lowByte() function

  Extracts the low (rightmost) 8 bits from a larger data type
  Useful when dealing with communication protocols (like I2C, SPI, serial) where data must be sent in bytes, or when manually splitting numbers

  This code return the "low" (least significant) byte (8-bit) from 16-bit integer

  Example :
  if x = 0xABCD; it will return CD
  if x = 0xCDAB; it will return AB
  if x = 0x85A6; it will return A6
*/

unsigned int x = 0x85A6; /* Hex variable */

void setup() {
  Serial.begin(115200);
  
  byte low = lowByte(x);  /* Extracts 0xAB */

  Serial.print("The low byte is: ");
  Serial.println(low, HEX);  /* Prints "CD" */
 
}

void loop() {
}
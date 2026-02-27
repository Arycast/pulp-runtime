/*
  Bits and Bytes API
  : bitClear() function

  Clears (writes a 0 to) a bit of a numeric variable at a specific position
  Useful for low-level bit manipulation, especially when working with hardware registers, flags, or memory-mapped I/O.

  This code Modify a given byte x by turning its 6th bit to 0

  Example :
  if index = 6; it will modify 6th bit and x = 11011111
  if index = 7; it will modify 7th bit and x = 10111111
*/

uint8_t x = 0b11111111; /* initial byte */

void setup() {
  Serial.begin(115200);
  
  int index = 7; /* index of the bit to modify */
  
  x = bitClear(x, index-1);

  Serial.print("The resulting byte is: ");
  Serial.println(x, BIN);
 
}

void loop() {
}
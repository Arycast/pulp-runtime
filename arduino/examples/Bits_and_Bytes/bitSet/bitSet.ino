/*
  Bits and Bytes API
  : bitSet() Function

  Sets (writes a 1 to) a bit of a numeric variable at a specific position
  Useful for low-level bit manipulation, especially when working with hardware registers, flags, or memory-mapped I/O

  This code Modify a given byte x by turning its 5th bit to 1

  Example :
  if index = 5; it will set the 5th bit to 1. x = 0b10010001
  if index = 2; it will set the 2nd bit to 1. x = 0b10000011
*/

uint8_t x = 0b10000001; /* initial byte */ 

void setup() {
  Serial.begin(115200);
  
  int index = 5; /* index of the bit to modify */
  
  x = bitSet(x, index-1);

  Serial.print("The resulting byte is: ");
  Serial.println(x, BIN);
 
}

void loop() {
}
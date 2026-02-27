/*
  Bits and Bytes API
  : bit() function

  Create a bitmask with a single bit set at a specific position
  Useful for bitwise operations, like setting, clearing, or testing specific bits in a byte or register

  This code modify a given byte x by turning its 6th bit to 1

  Example :
  if index = 6; it will modify 6th bit and x = 00100000
  if index = 7; it will modify 7th bit and x = 01000000
*/

uint8_t x = 0b00000000; /* initial byte */ 

void setup() {
  Serial.begin(115200);
  
  int index = 7; /* index of the bit to modify */
  
  x |= bit(index-1);

  Serial.print("The resulting byte is: ");
  Serial.println(x, BIN); /* leading zero got ignored in print result */
 
}

void loop() {
}
/*
  Bits and Bytes API
  : bitRead() function

  Reads a bit of a variable on a specific position, e.g. bool, int. 
  Note that float & double are not supported
  The bit of variables can be read up to an unsigned long long (64 bits / 8 bytes)

  This example code demonstrates how to read two variables, one increasing counter, one decreasing counter, and print out both the binary and decimal values of the variables

  Example :
  If use long_size when looping, it will return 32-bit of the two variables followed by their decimal value
  (e.g.)  Positive Binary Number: 00000000000000000000000000000000 Decimal Number: 0
          Negative Binary Number: 00000000000000000000000000000000 Decimal Number: 0 //First loop

          Positive Binary Number: 00000000000000000000000000000001 Decimal Number: 1
          Negative Binary Number: 11111111111111111111111111111111 Decimal Number: -1 //Second loop

  If use int_size when looping, it will return 8-bit of the two variables followed by their decimal value
  (e.g.)  Positive Binary Number: 00000000 Decimal Number: 0
          Negative Binary Number: 00000000 Decimal Number: 0 // First loop

          Positive Binary Number: 00000001 Decimal Number: 1
          Negative Binary Number: 11111111 Decimal Number: -1 // Second loop

  If use bool_size when looping, it will return 1-bit of the two variables followed by their decimal value
  (e.g.)  Positive Binary Number: 0 Decimal Number: 0
          Negative Binary Number: 0 Decimal Number: 0 //First loop

          Positive Binary Number: 1 Decimal Number: 1
          Negative Binary Number: 1 Decimal Number: -1 // Second loop


*/

long negative_var = -0;  /* negative variable */
unsigned long long positive_var = 0; /* positive variable */

/*  
    * predefined sizes when looping through bits
    * e.g. long_size is 32 bit (which is 0-31)
    */
const int bool_size = (1 - 1);
const int int_size = (8 - 1);
const int long_size = (32 - 1);

void setup() {
  Serial.begin(115200);
}

void loop() {
  /* run readBit function, passing the positive or negative variables */
  readBit("Positive ", positive_var);
  readBit("Negative ", negative_var);
  Serial.println();

  /* increase and decrease the variables */
  negative_var--;
  positive_var++;

  delay(1000);
}

/*  
    * readBit function
    * Takes a variable, prints it out bit by bit (starting from the right)
    * then prints the decimal number for comparison
    */
void readBit(String direction, long counter) {
  Serial.print(direction + "Binary Number: ");

  /* loop through each bit */
  for (int b = long_size; b >= 0; b--) {
    byte bit = bitRead(counter, b);
    Serial.print(bit);
  }
  Serial.print(" Decimal Number: ");
  Serial.println(counter);
}
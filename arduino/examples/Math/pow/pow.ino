/*
  Arduino Math API
  : pow() function

  Calculates the value of a number raised to a power
  This function can be used to raise a number to a fractional power

  This code calculate the value of x raised to the power of y

  Example :
  if x = 2.0; y = 10.0; it will return 1024.00
  if x = 2.0; y = 10.5; it will return 1448.15
*/

void setup() {
  Serial.begin(115200);

  float x = 2.0;  /* base number */
  float y = 10.0;  /* exponent number */

  double z = pow(x, y);

  Serial.print("The power result is: ");
  Serial.println(z);
}

void loop() {
}
/*
  Arduino Math API
  : sqrt() function

  Calculates the square root of a number

  This code calculate the square root of the x variable

  Example :
  if x = 9.00; it will return 3.00 because the square root of 9 is 3
  if x = 10.00; it will return 3.16 because the square root of 10 is 3.1622
  if x = 16.25; it will return 4.03 because the square root of 15.25 is 4.0311
*/

void setup() {
  Serial.begin(115200);
  
  float x = 16.25;  
  double root = sqrt(x);

  Serial.print("The square root is: ");
  Serial.println(root);
}

void loop() {
}
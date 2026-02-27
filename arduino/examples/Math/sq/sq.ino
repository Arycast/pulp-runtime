/*
  Arduino Math API
  : sq() function

  Calculates the square of a number (the number multiplied by itself)

  This code calculate the square of x

  Example :
  if x = 2; it will return 4 because 2*2 = 4
  if x = 2.5; it will return 6.25 because 2.5 * 2.5 = 6.25
  if x = 3.25; it will return 10.56 because 3.25 * 3.25 = 10.5625

  Notes and Warnings :
  Avoid using other functions inside the brackets
  (e.g.)  int inputSquared = sq(Serial.parseInt()); // avoid this

  Use this instead :
  (e.g.)  int input = Serial.parseInt();  // keep other operations outside the sq function
          int inputSquared = sq(input);
*/

void setup() {
  Serial.begin(115200);

  float x = 2.00;  
  double square = sq(x);

  Serial.print("The result is: ");
  Serial.println(square);
}

void loop() {
}
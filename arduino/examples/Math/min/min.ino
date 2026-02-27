/*
  Arduino Math API
  : min() function

  Find the smaller of two numbers

  This code compares a and b and print the smaller variable in the Serial Monitor

  Example :
  if a = 25; b = 14; This function will return 14 because 14 < 25

  Another typical application could be to constrain a maximum value of a variable
  (e.g.)  sensVal = min(sensVal, 100);  // assigns sensVal to the smaller of sensVal or 100
                                        // ensuring that it never gets above 100.

  Notes and Warnings :
  Avoid using other functions inside the brackets
  (e.g.)  min(a++, 100);  // avoid this - yields incorrect results

  Use this instead :
  (e.g.)  min(a, 100);
          a++;  // keep other math outside the function
*/

void setup() {
  Serial.begin(9600);

  int a = 25;
  int b = 14;

  int minVal = min(a, b);

  Serial.print("The smaller value is: ");
  Serial.println(minVal);
}

void loop() {
}
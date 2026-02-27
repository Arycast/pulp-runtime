/*
  Arduino Math API
  : max() function

  Find the larger of two numbers

  This code compares a and b, then print the larger variable in the Serial Monitor

  Example :
  if a = 25; b = 14; This function will return 25 because 25 > 14

  Another typical application could be to constrain a minimum value of a variable
  (e.g.)  sensVal = max(sensVal, 20); // assigns sensVal to the larger of sensVal or 20
                                      // (effectively ensuring that it is at least 20)
  
  Notes and Warnings :
  Avoid using other functions inside the brackets
  (e.g.)  max(a--, 0);  // avoid this - yields incorrect results

  Use this instead :
  (e.g.)  max(a, 0);
          a--; // keep other math outside the function
*/

void setup() {
  Serial.begin(9600);

  int a = 25;
  int b = 14;
  int maxVal = max(a, b);

  Serial.print("The larger value is: ");
  Serial.println(maxVal);
}

void loop() {
}
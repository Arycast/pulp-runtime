/*
  Arduino Math API
  : abs() function

  The function calculates the absolute value of given number

  Example :
  if int x = 42, so abs(x) will return 42
  if int x = -42, so abs(x) will return 42

  Notes and warning :
  Because of the way the abs() function is implemented, avoid using other functions inside the brackets, it may lead to incorrect results
  (e.g.)  abs(x++); //avoid this - yield incorrect results

  use this instead :
  (e.g.)  abs(x);
          x++; //keep other math outside the function

  https://docs.arduino.cc/language-reference/en/functions/math/abs/
*/

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  int x = 42;
  Serial.print("The absolute value of ");
  Serial.print(x);
  Serial.print(" is ");
  Serial.println(abs(x));
  x = -42;
  Serial.print("The absolute value of ");
  Serial.print(x);
  Serial.print(" is ");
  Serial.println(abs(x));
}

void loop() {
}
/*
  Arduino Math API
  : constrain() function

  Constraints a number to be within a range

  This code sets an emulated sensor value and prints its constrained result on the Serial Monitor.

  Example :
  * constrain(sensVal, 10, 150)
  if sensVal = 50, it will return 50
  if sensVal = 170, it will return 150
  if sensVal = 5, it will return 10

  Notes and Warnings :
  avoid using other functions inside the brackets, it may lead to incorrect results
  (e.g.)  int constrainedInput = constrain(Serial.parseInt(), minimumValue, maximumValue); // avoid this
  
  use this instead :
  (e.g.)  int input = Serial.parseInt(); //keep other operations outside the constrain function
          int constrainedInput = constrain(input, minimumValue, maximumValue);
  
  https://docs.arduino.cc/language-reference/en/functions/math/constrain/
*/

int sensVal = 50;  // try with 170 and 5 to verify the functionality

void setup() {
  Serial.begin(115200);

  sensVal = constrain(sensVal, 10, 150);  // limits range of sensor values between 10 and 150

  Serial.print("The sensor value is: ");
  Serial.println(sensVal);
}

void loop() {
}
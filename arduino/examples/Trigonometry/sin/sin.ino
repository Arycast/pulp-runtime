/*
  Trigonometry API
  : sin() function

  Calculates the sine of an angle (in radians)
  The result will be between -1 and 1

  This code will return the sine value (in decimals) of an angle

  Example :
  if angle = 60; it will return 0.87
  if angle = 90; it will return 1
  if angle = 45; it will return 0.71
*/

float angle = 90;
float radians = (angle / 180) * M_PI;  /*deegres in radians*/
double res = 0;

void setup() {
  Serial.begin(115200);

  res = sin(radians);

  Serial.print("The angle sine is: ");
  Serial.println(res);
}

void loop() {
}
/*
  Trigonometry API
  : tan() function

  Calculates the tangent of an angle (in radians)
  The result will be between negative infinity and infinity

  This code will return the tangent value (in decimals) of an angle

  Example :
  if angle = 60; it will return 1.73
  if angle = 90; it will return -22877332.00 (negative infinity)
  if angle = 45; it will return 1
*/

float angle = 45;
float radians = (angle / 180) * M_PI;  /*deegres in radians*/
double res = 0;

void setup() {
  Serial.begin(115200);

  res = tan(radians);

  Serial.print("The angle tangent is: ");
  Serial.println(res);
}

void loop() {
}
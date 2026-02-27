/*
  Trigonometry API
  : cos() function

  Calculates the cosine of an angle (in radians)
  The result will be between -1 and 1

  This code will return the cosine value (in decimals) of an angle

  Example :
  if angle = 60; it will return 0.50
  if angle = 90; it will return 0
  if angle = 45; it will return 0.71
*/

float angle = 45;
float radians = (angle / 180) * M_PI;  /* deegres in radians*/
double res = 0;

void setup() {
  Serial.begin(115200);

  res = cos(radians);

  Serial.print("The angle cosine is: ");
  Serial.println(res);
}

void loop() {
}
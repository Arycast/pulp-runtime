/*
  Arduino Math API
  : map() function
      * map(value, fromLow, fromHigh, toLow, toHigh)

  Re-maps a number from one range to another
  That is, a value of fromLow would get mapped to toLow, a value of fromHigh to toHigh, values in-between to values in-between
  Does not constrain values to within the range, because out-of-range values are sometimes intended and useful

  This code maps a 12-bit (0-4095) input value to 8-bit (0-255) value
  The input and output values are printed in the Serial Monitor

  Example :
  * map(x, 0, 4095, 0, 255);
  if x = 3855, it will return 240 instead of 240.05
  if x = 940, it will return 58 instead of 58.53
  if x = 255, it will return 15 instead of 15.87
  
  This can be used to remap raw analog input value (e.g. using potentiometer to control brightness of a led with pwm)
  (e.g.)  int x = analogRead(pin_analog_input);
          val = map(x, 0, 4095, 0, 255);
          analogWrite(pin_pwm, val);

  Notes and Warnings :
  "lower bounds" of either range may be larger or smaller than the "upper bounds", so the function may be used to reverse a range of numbers
  (e.g.)  map(x, 1, 50, 50, 1);

  This function also handles negative numbers well
  (e.g.)  map(x, 1, 50, 50, -100);
  
  The function uses integer math so will not generate fractions, when the math might indicate that it should do so
  Fractional remainders are truncated, and are not rounded or averaged
  (e.g.)  map(20, 1, 50, 50, 10); //it will return 34 instead of 34.489
*/

void setup() {
  Serial.begin(115200);
}

void loop() {
  int val = 3855; /* try with 940 and 255 to get another result */

  Serial.print("Input value: ");
  Serial.print(val);

  val = map(val, 0, 4095, 0, 255);

  Serial.print(", Output value: ");
  Serial.println(val);
}
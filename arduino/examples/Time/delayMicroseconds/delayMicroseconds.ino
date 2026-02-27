/*
  Arduino Time API
  : delayMicroseconds() function

  Pauses the program for the amount of time (in microseconds) specified by the parameter
  There are a thousand microseconds in a millisecond and a million microseconds in a second

  The largest value that will produce an accurate delay is 16383
  larger values can produce an extremely short delay
  For delays longer than a few thousand microseconds, use delay() instead

  This code pauses the program for 50 microseconds before toggling the LED_BUILTIN pin

  Example :
  if delayMicroseconds(50); the program will pause for 50 us
  if delayMicroseconds(10); the program will pause for 10 us
  if delayMicroseconds(25); the program will pause for 25 us 
*/

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); /* sets the LED_BUILTIN pin as output */
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  /* sets the LED on */
  delayMicroseconds(50);            /* waits for 50 microsecond */
  digitalWrite(LED_BUILTIN, LOW);   /* sets the LED off */
  delayMicroseconds(50);            /* waits for 50 microsecond */
}
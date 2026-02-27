/*
  Arduino Time API
  : delay() function

  Pauses the program for the amount of time (in milliseconds) specified as parameter

  This code pauses the program for one second before toggling the LED_BUILTIN pin

  Example :
  if delay(1000); the program will pause for 1000 ms (1 second)
  if delay(100); the program will pause for 100 ms (0.1 second)
  if delay(10000); the program will pause for 10000 ms (10 seconds)

  Notes and Warnings :
  Besides its "easy to use", the use of this function has significant drawbacks
  No other reading of sensors, mathematical calculations, or pin manipulation can occur during the delay function, so, in effect, it brings most other activity to a halt

  For alternative approaches to controlling timing, use mills() polling in a loop
  More knowledgeable programmers usually avoid using delay() for timing events longer than 10s of milliseconds
*/

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); /* sets the LED_BUILTIN pin as output */
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  /* sets the LED on */
  delay(1000);                      /* waits for a second (1000 ms) */
  digitalWrite(LED_BUILTIN, LOW);   /* sets the LED off */
  delay(1000);                      /* waits for a second (1000 ms) */
}
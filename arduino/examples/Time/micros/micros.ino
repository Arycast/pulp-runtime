/*
  Arduino Time API
  : micros() function

  Returns the number of microseconds since the Arduino board began running the current program
  This number will overflow (go back to zero) after approximately 70 minutes

  This code returns the number of microseconds since the Arduino board began running the current program

  Notes :
  There are 1,000 microseconds in a millisecond and 1,000,000 microseconds in a second
*/

unsigned long time;

void setup() {
  Serial.begin(115200);
}
void loop() {
  Serial.print("Time: ");
  time = micros();

  Serial.println(time); /* prints time since program started */
  delay(1000);          /* wait a second so as not to send massive amounts of data */
}
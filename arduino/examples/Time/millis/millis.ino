/*
  Arduino Time API
  : millis() function

  Returns the number of milliseconds passed since the Arduino board began running the current program
  This number will overflow (go back to zero), after approximately 50 days

  This example code prints on the serial monitor the number of milliseconds passed since the board started running the code itself

  Notes and Warnings :
  * The return value for millis() is unsigned long 
    Logic errors may occur if a programmer tries to do arithmetic with smaller data types such as int 
    Even signed long may encounter errors as its maximum value is half that of its unsigned counterpart

  * millis() will wrap around to 0 after about 50 days (micros in about 70 minutes)

  * Reconfiguration of the microcontroller’s timers may result in inaccurate millis() readings 
*/

unsigned long myTime;

void setup() {
  Serial.begin(115200);
}
void loop() {
  Serial.print("Time: ");
  myTime = millis();

  Serial.println(myTime); /* prints time since program started */
  delay(1000);          /* wait a second so as not to send massive amounts of data */
}
/*
  Serial Communication API
  : Serial.begin() function

  Sets the data rate in bits per second (baud) for serial data transmission

  Notes :
  Default baudrate is 115200
  Adjusting baudrate and config in Serial.begin() don't affect anything for now
  Make sure the baudrate in serial monitor is set to 115200 to read serial data
*/

void setup() {
    Serial.begin(115200); /* opens serial port, sets data rate to 115200 bps */
}

void loop() {}
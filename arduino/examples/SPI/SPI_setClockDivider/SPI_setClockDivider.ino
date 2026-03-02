/*
  SPI Communication API
  : SPI.setClockDivider() function (Deprecated)

  Sets the SPI Frequency to get clock divider relative to the system clock

  Default frequency is 1000000

  Note : 
  This function is deprecated and should not be used in new projects
  Use SPISettings with SPI.beginTransaction() to configure SPI parameters
    (e.g.) SPI.beginTransaction(SPISettings(1000000, SPI_MSBFIRST, SPI_MODE0));
*/

uint8_t send_data = 0xAF; /* Dummy data to send*/

void setup() {
  Serial.begin(115200);

  SPI.begin();

  /* DEPRECATED */
  /* Use SPISettings with SPI.beginTransaction() instead */
  SPI.setClockDivider(5000000); /* Set the frequency to get clock divider */

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(send_data);
  digitalWrite(PIN_SPI_CS, HIGH);

  Serial.print("Data terkirim");
}

void loop() {


}

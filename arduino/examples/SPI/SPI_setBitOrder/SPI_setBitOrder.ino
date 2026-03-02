/*
  SPI Communication API
  : SPI.setBitOrder() function (Deprecated)

  Sets the order of the bits shifted out of and into the SPI bus
  SPI_LSBFIRST (least-significant bit first) or SPI_MSBFIRST (most-significant bit first)

  Default bitOrder is SPI_MSBFIRST

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
  SPI.setBitOrder(SPI_MSBFIRST); /* Set the bit order */

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(send_data);
  digitalWrite(PIN_SPI_CS, HIGH);

  Serial.print("Data terkirim");
}

void loop() {


}

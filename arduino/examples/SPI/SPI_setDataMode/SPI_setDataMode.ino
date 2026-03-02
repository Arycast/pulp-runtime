/*
  SPI Communication API
  : SPI.setDataMode() function (Deprecated)

  Sets the SPI data mode, which defines the clock polarity (CPOL) and clock phase (CPHA)

  CPHA = Clock Phase, CPOL = Clock Polarity
  SPI_MODE0 -> CPHA = 0, CPOL = 0
  SPI_MODE1 -> CPHA = 1, CPOL = 0
  SPI_MODE2 -> CPHA = 0, CPOL = 1
  SPI_MODE3 -> CPHA = 1, CPOL = 1 

  Default SPI data mode is SPI_MODE0

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
  SPI.setDataMode(SPI_MODE0); /* Set the data mode */

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(send_data);
  digitalWrite(PIN_SPI_CS, HIGH);

  Serial.print("Data terkirim");
}

void loop() {


}

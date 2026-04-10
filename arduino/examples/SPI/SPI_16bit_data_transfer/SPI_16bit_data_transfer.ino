/*
  16-bit (2-byte) data transfer using SPI

  This code will send 16-bit (2-byte) dummy data using SPI

  Note :
  This method also return 16-bit return value
  Try to store the return value to a 16-bit variable
*/

uint16_t data = 0xABCD; /* Dummy data to send */
uint16_t receivedData; /* 16-bit variable to store return value*/

void setup() {
  pinMode(PIN_SPI_CS, OUTPUT);
  SPI.begin();

  SPI.beginTransaction(SPISettings(1000000, SPI_MSBFIRST, SPI_MODE0));
  digitalWrite(PIN_SPI_CS, HIGH);

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer16(data);
  // receivedData = SPI.transfer16(data); /* Try this */
  digitalWrite(PIN_SPI_CS, HIGH);
  SPI.endTransaction();
}

void loop() {
  
}

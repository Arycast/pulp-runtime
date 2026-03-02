/*
  Simple data transfer using SPI

  This code will send a dummy data using SPI
*/

uint8_t send_data = 0xAA; /* Dummy data to send*/

void setup() {
  Serial.begin(115200);

  SPI.begin();
  SPI.beginTransaction(SPISettings());

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(send_data);
  // SPI.transfer(0xAA); /* Inline data/instruction injection (for single byte) */
  digitalWrite(PIN_SPI_CS, HIGH);

  Serial.print("Data terkirim");
}

void loop() {


}

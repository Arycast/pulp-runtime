/*
  Single data transfer using SPI

  This code will send a dummy data using SPI

  Note :
  This code also can return 8-bit return value
  Try to store the return value to a 8-bit variable

  This method can be used in loop
*/

uint8_t send_data = 0xAA; /* Dummy data to send */
uint8_t receivedData;

void setup() {
  Serial.begin(115200);

  SPI.begin();
  SPI.beginTransaction(SPISettings());

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(send_data);
  // SPI.transfer(0xAA); /* Inline data/instruction injection (for single byte) */
  // receivedData = SPI.transfer(send_data); /* Try this */
  digitalWrite(PIN_SPI_CS, HIGH);

  Serial.print("Data terkirim");
}

void loop() {


}

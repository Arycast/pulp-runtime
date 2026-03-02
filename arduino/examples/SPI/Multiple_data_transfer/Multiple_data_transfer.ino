/*
  Multiple data transfer using SPI

  This code will send multiple dummy data using SPI
*/

uint8_t send_data[4] = {0xFF, 0xFF, 0xFF, 0xFF}; /* Dummy data to send*/

void setup() {
  Serial.begin(115200);

  SPI.begin();
  SPI.beginTransaction(SPISettings());

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(send_data, sizeof(send_data));
  digitalWrite(PIN_SPI_CS, HIGH);

  Serial.print("Data terkirim");
}

void loop() {


}

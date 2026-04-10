/*
  Reads the JEDEC ID from W25Q64JV-SIQ flash memory chip

  Code details :
  1. Send the instruction code : 0x9F (Read JEDEC ID)
  2. Send 3-byte dummy data using rx_buffer for storing the input data inside the rx_buffer

  Received Data :
    * Manufacturer ID (0xEF)
    * Memory Type ID (0x40)
    * Capacity ID (0x17)

  Reference : W25Q64JVSIQ Datasheet, Section 8.3.12 "Read JEDEC ID (9Fh)"

  https://www.winbond.com/hq/new-online-purchasing-guide/?__locale=en&pLine=/product/code-storage-flash/qspi-nor/&pNo=W25Q64JV
*/

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SPI_CS, OUTPUT);
  SPI.begin();

  Serial.println("-----Program Started-----");

  SPI.beginTransaction(SPISettings(1000000, SPI_MSBFIRST, SPI_MODE0)); /* 1 MHz, MSB first, mode 0 */

  uint8_t cmd[1] = {0x9F};
  uint8_t rx_buffer[3] = {0x00, 0x00, 0x00};

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(cmd, sizeof(cmd));
  SPI.transfer(rx_buffer, sizeof(rx_buffer));
  digitalWrite(PIN_SPI_CS, HIGH);
  SPI.endTransaction();

  Serial.print("Manufacturer ID : 0x");
  Serial.print(rx_buffer[0], HEX);
  Serial.print("\t Memory Type ID : 0x");
  Serial.print(rx_buffer[1], HEX);
  Serial.print("\t Capacity ID : 0x");
  Serial.println(rx_buffer[2], HEX);
}

void loop() {

}



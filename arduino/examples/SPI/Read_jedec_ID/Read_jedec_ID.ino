/*
  Reads the JEDEC ID from W25Q64JV flash memory chip

  Code details :
  1. Send instruction code : 0x9F (Read JEDEC ID)
  2. Receive Data :
      * Manufacturer ID (Expected 0xEF for Winbond)
      * Memory Type ID
      * Capacity ID 

  The return IDs will be stored in the "buffer[4]"

  Reference : W25Q64JV Datasheet, Section 8.3.12 "Read JEDEC ID (9Fh)"
*/

static L2_DATA uint8_t buffer[4] = {0x9F, 0x00, 0x00, 0x00};

void setup() {
  pinMode(PIN_SPI_CS, OUTPUT);
  SPI.begin();

  SPI.beginTransaction(SPISettings(1000000, SPI_MSBFIRST, SPI_MODE0)); /* 1 MHz, MSB first, mode 0 */
  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(buffer, sizeof(buffer));
  digitalWrite(PIN_SPI_CS, HIGH);
  SPI.endTransaction();
}

void loop() {

}



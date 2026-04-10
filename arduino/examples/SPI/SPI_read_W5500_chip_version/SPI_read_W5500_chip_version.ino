/*
  Reads the chip version of W5500 Ethernet Module

  Code details :
  1. Send 16-bit (2 byte) address phase : 0x0039 (offset address for chip version)
  2. Send 8-bit (1 byte) control phase : 0x00
      - [7:3] Block Select Bits (BSB) : Select common register (00000)
      - [2] R/W access mode bit : Set to read mode (0)
      - [1:0] SPI operation mode bits : Set to variable data length mode (00)
  3. Send 1 byte dummy data using SPI single byte transfer method and get the return value
  4. Received data = 4 (W5500 chip version)

  Reference : W5500 Datasheet V1.0.2, Section 4.1 Common Registers | VERSIONR

  https://cdn.sparkfun.com/datasheets/Dev/Arduino/Shields/W5500_datasheet_v1.0.2_1.pdf
*/

uint8_t tx_data[3] = {0x00, 0x39, 0x00};
uint8_t dummy;
uint8_t receivedData;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SPI_CS, OUTPUT);
  SPI.begin();

  digitalWrite(PIN_SPI_CS, HIGH);

  SPI.beginTransaction(SPISettings(10000000, SPI_MSBFIRST, SPI_MODE0));
  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(tx_data, sizeof(tx_data));
  receivedData = SPI.transfer(dummy);
  digitalWrite(PIN_SPI_CS, HIGH);

  Serial.println(receivedData);

}

void loop() {
}

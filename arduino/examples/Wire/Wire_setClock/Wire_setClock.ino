/* Testing the success of clock-frequency-changing for every 3 seconds */

#define EEPROM_ADDR 0xD0
#define DATA_LENGTH 4

uint8_t tx_data[DATA_LENGTH] = {0x12, 0x34, 0x56, 0x78};

void setup() {
  Wire.begin();
}

void loop() {
  // Standard Mode (100 kHz). Oscilloscope will measured the frequency as 41 kHz.
  Wire.setClock(100000);
  sendI2CPacket();
  delay(3000);

  // Fast Mode (400 kHz). Oscilloscope will measured the frequency as 131 kHz.
  Wire.setClock(400000);
  sendI2CPacket();
  delay(3000);

  /* Fast mode plus (1 Mhz) and high speed mode (3.4 MHz) is not supported */
}

void sendI2CPacket() {
  Wire.beginTransmission(EEPROM_ADDR);  // Transmit to device number 0xD0
  Wire.write(0x08);  // Send this to access DS1307 ram starting address
  Wire.write(tx_data, DATA_LENGTH);
  Wire.endTransmission();      // Stop transmitting
}
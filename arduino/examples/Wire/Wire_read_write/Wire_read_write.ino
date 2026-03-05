/* Write and read 4 bytes data using DS1307 as slave */

#define EEPROM_ADDR 0xD0
#define DATA_LENGTH 4

uint8_t tx_data[DATA_LENGTH] = {0x12, 0x34, 0x56, 0x78};
uint8_t rx_data[DATA_LENGTH];

int i;

void setup() {
  Wire.begin();

  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(0x08);  // Send this to access DS1307 ram starting address
  Wire.write(tx_data, DATA_LENGTH);
  Wire.endTransmission();

  for (int i; i<1000000; i++);

  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(0x08);
  Wire.endTransmission();

  Wire.requestFrom(EEPROM_ADDR, DATA_LENGTH);
  int i = 0;
  while (Wire.available() && i < DATA_LENGTH) {
    rx_data[i++] = Wire.read();
  }
  
  Serial.print("EEPROM read: ");
  for (int j = 0; j < DATA_LENGTH; j++) {
    Serial.print("0x");
    Serial.print(rx_data[j], HEX);
    Serial.print(" ");
  }
  Serial.println();
  Wire.end();
}
void loop() {

}
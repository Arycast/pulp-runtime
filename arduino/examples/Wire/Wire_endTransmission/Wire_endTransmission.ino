/* Various Wire.endTransmission-return scenarios based on https://docs.arduino.cc/language-reference/en/functions/communication/wire/endTransmission/ */
/* Joining the SCL pin and SDA pin with pull down resistor can create an alternative timeout situation. */

uint8_t tx_data[4] = {0x12, 0x34, 0x56, 0x78};
uint8_t tx_data_1[33] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0};

void setup() {
  Serial.begin(115200); // Pastikan baudrate sesuai dengan terminal monitor kamu
  Wire.begin();
}

void loop() {
  // Return 0 to indicate success
  Wire.beginTransmission(0xD0); // DS1307 deivce address
  Wire.write(0x08); // DS1307 word address
  Wire.write(tx_data, 4);
  Serial.println(Wire.endTransmission());

  delay(15000);

  // Return 1 to indicate data too long to fit in transmit buffer
  Wire.beginTransmission(0xD0);
  Wire.write(0x08);
  Wire.write(tx_data_1, 33);
  Serial.println(Wire.endTransmission());

  delay(15000);

  // Return 5 to indicate timeout. 
  #if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(3000 /* us */, true /* reset_on_timeout */);
  #endif  
  Wire.beginTransmission(0xD0);
  Wire.write(0x08);
  Wire.write(tx_data, 4);
  Serial.println(Wire.endTransmission());
  #if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(0);
  #endif  

  delay(15000); 
}
/* Using various Wire.write syntax based on https://docs.arduino.cc/language-reference/en/functions/communication/wire/write/*/
/* Set the decoding format data in your oscilloscope for better decode results */

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("I2C Master Write Test Ready");
}

void loop() {
  // // DS1307 address as the targeted peripheral
  const uint8_t SLAVE_ADDR = 0xD0;

  // --- 1. Syntax: Wire.write(value) ---
  // Send a value as a single byte
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(123); // Showed as 0x7B in HEX
  Wire.endTransmission();
  
  delay(500);

  // --- 2. Syntax: Wire.write(string) ---
  // Send a string as a series of bytes
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write("Hello"); // Send "H", "e", "l", "l", "o". Showed as "48", "65", "6C", "6C", "6F" in HEX.
  Wire.endTransmission();

  delay(500);

  // --- 3. Syntax: Wire.write(data, length) ---
  // Send an array of data as bytes
  uint8_t buffer[] = {10, 20, 30, 40, 50}; // Showed as "0A", "14", "1E", "28", "32" in HEX
  uint8_t length = 5;

  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(buffer, length);
  Wire.endTransmission();

  Serial.println("One testing cycle has finished.");
  delay(2000);
}
/* The example is taken from  https://docs.arduino.cc/language-reference/en/functions/communication/wire/read/
and modified as the slave is DS1307 */

void setup() {
  Wire.begin();        // Join I2C bus (address is optional for controller device)
  Serial.begin(115200);  // Start serial for output
}

void loop() {
  // Send 0x08 as the first byte to access the address where the value is written
  Wire.beginTransmission(0xD0);
  Wire.write(0x08);
  Wire.endTransmission();
  
  Wire.requestFrom(0xD0, 6);  // Request 6 bytes from peripheral device 0xD0 (DS1307)

  // Peripheral may send less than requested
  while (Wire.available()) {
    char c = Wire.read();  // Receive a byte as character
    Serial.print(c);       // Print the character
  }

  delay(500);
}
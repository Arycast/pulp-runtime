/* The example is taken from  https://docs.arduino.cc/language-reference/en/functions/communication/wire/write/ 
and modified as the slave is DS1307 */

byte word_address = 0x08;        // The ram address start from 0x08 in DS1307
byte val = 0;

void setup() {
  Wire.begin();  // Join I2C bus
}

void loop() {
  Wire.beginTransmission(0xD0);  // Transmit to device number 0xD0
  Wire.write(word_address);    // Send word address as the first byte
  Wire.write(val);             // Send value byte
  Wire.endTransmission();      // Stop transmitting

  word_address++;
  val++;  // Increment value

  // If reached 0x3F position (ram max address), start over from lowest value
  if (word_address > 0x3F) {
    word_address = 0x08;
    val = 0;
  }

  delay(500);
}
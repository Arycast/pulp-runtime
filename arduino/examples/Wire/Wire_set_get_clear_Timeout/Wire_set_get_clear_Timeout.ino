/* The example is taken from  https://docs.arduino.cc/language-reference/en/functions/communication/wire/write/ 
and modified as the slave is DS1307 */
/* Joining the SCL pin and SDA pin with pull down resistor can create an alternative timeout situation. */

void setup() {
  Wire.begin();  // Join I2C bus (address optional for controller)
  #if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(3000 /* us */, true /* reset_on_timeout */);
  #endif
}

void loop() {
  // Send a command to the peripheral device
  Wire.beginTransmission(0xD0); // Address 0xD0 belongs to DS1307
  Wire.write(0x08); // Address 0x08 is where the ram starts in DS1307
  Wire.write(123);
  byte error = Wire.endTransmission();
  if (error) {
    Serial.println("Error occurred when writing");
    if (error == 5)
      Serial.println("It was a timeout");
  }  
  delay(10000);

  // Read the result
  #if defined(WIRE_HAS_TIMEOUT)
  Wire.clearWireTimeoutFlag();
  #endif

  // This part added as DS1307 required device address as the first byte before DS1307 transmitting data to master
  Wire.beginTransmission(0xD0);
  Wire.write(0x08);
  Wire.endTransmission();
  // end of additional part

  byte len = Wire.requestFrom(0xD0, 1);
  if (len == 0) {
    Serial.println("Error occurred when reading");
    #if defined(WIRE_HAS_TIMEOUT)
    if (Wire.getWireTimeoutFlag())
      Serial.println("It was a timeout");
    #endif
  }

  delay(10000);
}
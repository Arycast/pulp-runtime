/* Write and read 4 bytes data using DS1307 as slave */

#define EEPROM_ADDR 0xD0
#define DATA_LENGTH 4

uint8_t tx_data[DATA_LENGTH] = {0x12, 0x34, 0x56, 0x78};
uint8_t rx_data[DATA_LENGTH];

unsigned long millisVar;
bool running = true;

int i;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("-----i2c Started-----");
}
void loop() {
  if (running == true) {
    millisVar = millis();

    Wire.beginTransmission(EEPROM_ADDR);
    Wire.write(0x08);  // Send this to access DS1307 ram starting address
    Wire.write(tx_data, DATA_LENGTH);
    Wire.endTransmission();

    delay(500);

    Wire.beginTransmission(EEPROM_ADDR);
    Wire.write(0x08);
    Wire.endTransmission();

    Wire.requestFrom(EEPROM_ADDR, DATA_LENGTH);
    int i = 0;
    while (Wire.available() && i < DATA_LENGTH) {
      rx_data[i++] = Wire.read();
    }
    
    Serial.print("EEPROM read: ");
    for (int i = 0; i < DATA_LENGTH; i++) {
      Serial.print("0x");
      Serial.print(rx_data[i], HEX);
      Serial.println(" ");
    }

    if (rx_data[0] != tx_data[0] || rx_data[1] != tx_data[1] || rx_data[2] != tx_data[2] || rx_data[3] != tx_data[3]) {
      Serial.println("-----i2c Stopped-----");

      unsigned long hours = millisVar / 3600000;
      unsigned long remaining_ms = millisVar % 3600000;

      unsigned long minutes = remaining_ms / 60000;
      remaining_ms = remaining_ms % 60000;

      unsigned long seconds = remaining_ms / 1000;

      Serial.print("Program stopped after : ");
      Serial.print(hours);
      Serial.print(" Hours ");
      Serial.print(minutes);
      Serial.print(" Minutes ");
      Serial.print(seconds);
      Serial.println(" Seconds ");

      running = false;
    }

    delay(2000);
  }
}


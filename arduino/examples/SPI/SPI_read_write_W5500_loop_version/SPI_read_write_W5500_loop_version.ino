/*
  Write and read the Source IP Adress in W5500 Source IP Adress register (SIPR)
  "Loop Version"

  Code details :
  * Write source IP address
    1. Send 16-bit (2 byte) address phase : 0x000F (offset address for SIPR)
    2. Send 8-bit (1 byte) control phase : 0x04
        - [7:3] Block Select Bits (BSB) : Select common register (00000)
        - [2] R/W access mode bit : Set to write mode (1)
        - [1:0] SPI operation mode bits : Set to variable data length mode (00)
    3. Send the Source IP address (4-byte) to write in SIPR

  * Read source IP address
    1. Send 16-bit (2 byte) address phase : 0x000F (offset address for SIPR)
    2. Send 8-bit (1 byte) control phase : 0x00
        - [7:3] Block Select Bits (BSB) : Select common register (00000)
        - [2] R/W access mode bit : Set to read mode (0)
        - [1:0] SPI operation mode bits : Set to variable data length mode (00)
    3. Send the 4-byte dummy data to get the return Value in the same buffer

  Note :
  * Be careful for using SPI.transfer(buffer, size) in loop
    The return value will be stored in the same buffer and replaced the existing "tx_cmd and send_data" value, also "rx_cmd" value
    Buffer overflow will be happened in the second loop and so on

  * There are 2 options to resolve this :
    
    - First option
        Declare "tx_cmd[3], sendData[4], and rx_cmd[3]" variables inside the loop (Make it become local variables instead of global variables)
    
    - Second option
        Try to combine this method with single data SPI transfer method or 16-bit data SPI transfer method
        Use single SPI transfer method "multiple times" for fixed data value
        In this case : tx_cmd[3], sendData[4], and rx_cmd[3]

  Reference : W5500 Datasheet V1.0.2, Section 4.1 Common Registers | SIPR

  https://cdn.sparkfun.com/datasheets/Dev/Arduino/Shields/W5500_datasheet_v1.0.2_1.pdf
*/

unsigned long  millisVar;
bool running = true;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SPI_CS, OUTPUT);
  SPI.begin();
  Serial.println("-----Program Started-----");

  digitalWrite(PIN_SPI_CS, HIGH);
}

void loop() {
  if (running == true) {
    millisVar = millis();

/* ===== 1st Option ===== */
  
    /**
        * Declare tx_cmd, sendData, and rx_cmd as global variables (Inside "loop")
        */

    uint8_t tx_cmd[3] = {0x00, 0x0F, 0x04};
    uint8_t sendData[4] = {192, 168, 0, 2};
    uint8_t rx_cmd[3] = {0x00, 0x0F, 0x00};
    uint8_t receiveData[4] = {0x00, 0x00, 0x00, 0x00};

    SPI.beginTransaction(SPISettings(1000000, SPI_MSBFIRST, SPI_MODE0));
    digitalWrite(PIN_SPI_CS, LOW);

    /**
        * SPI Transfer (Write)
        * Use multiple data SPI transfer method for tx_cmd and sendData variables 
        */
    SPI.transfer(tx_cmd, sizeof(tx_cmd));
    SPI.transfer(sendData, sizeof(sendData));

    digitalWrite(PIN_SPI_CS, HIGH);
    SPI.endTransaction();

    SPI.beginTransaction(SPISettings(1000000, SPI_MSBFIRST, SPI_MODE0));
    digitalWrite(PIN_SPI_CS, LOW);

    /**
        * SPI Transfer (Read)
        * Use multiple data SPI transfer method for rx_cmd and receiveData variables 
        */
    SPI.transfer(rx_cmd, sizeof(rx_cmd));
    SPI.transfer(receiveData, sizeof(receiveData));

    digitalWrite(PIN_SPI_CS, HIGH);
    SPI.endTransaction();

/* ===== End of 1st Option ===== */

   /* ===== ----------------- ===== */

/* ===== 2nd Option ===== */

    // SPI.beginTransaction(SPISettings(1000000, SPI_MSBFIRST, SPI_MODE0));
    // digitalWrite(PIN_SPI_CS, LOW);

    /**
        * SPI Transfer (Write)
        * Use single data SPI transfer method "multiple times" for tx_cmd and sendData values
        */
    // SPI.transfer(0x00);
    // SPI.transfer(0x0F);
    // SPI.transfer(0x04);
    // SPI.transfer(192);
    // SPI.transfer(168);
    // SPI.transfer(0);
    // SPI.transfer(2);
    
    // digitalWrite(PIN_SPI_CS, HIGH);
    // SPI.endTransaction();

    // SPI.beginTransaction(SPISettings(1000000, SPI_MSBFIRST, SPI_MODE0));
    // digitalWrite(PIN_SPI_CS, LOW);

    /**
        * SPI Transfer (Read)
        * Use single data SPI transfer method "multiple times" for rx_cmd values
        * Use multiple data SPI transfer method receiveData variable
        */
    // SPI.transfer(0x00);
    // SPI.transfer(0x0F);
    // SPI.transfer(0x00);
    // SPI.transfer(receiveData, sizeof(receiveData));

    // digitalWrite(PIN_SPI_CS, HIGH);
    // SPI.endTransaction();

/* ===== End of 2nd Option ===== */

    for (int i = 0; i < sizeof(receiveData); i++) {
      Serial.println(receiveData[i]);
    }

    if (receiveData[0] != 192 || receiveData[1] != 168 || receiveData[2] != 0 || receiveData[3] != 2) {
      Serial.println("-----Program Stopped-----");

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

/*
  Write and read the Source IP Address in W5500 Source IP Adress register (SIPR)

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
  * Be careful for re-create this in loop
    The return value will be stored in the same buffer and replaced the existing "tx_cmd and send_data" value, also "rx_cmd" value
    Buffer overflow will be happened in the second loop and so on

  * There are 2 options to resolve this :
    
    - First option
        Declare "tx_cmd[3], sendData[4], and rx_cmd[3]" variables inside the loop (Make it become a local variable instead of a global variable)
    
    - Second option
        Try to combine this method with single data SPI transfer method or 16-bit data SPI transfer method
        Use single SPI transfer method multiple times for fixed data value
        In this case : tx_cmd[3], sendData[4], and rx_cmd[3]

  * See the loop version of this example in SPI_read_write_W5500_loop_version.ino

  Reference : W5500 Datasheet V1.0.2, Section 4.1 Common Registers | SIPR

  https://cdn.sparkfun.com/datasheets/Dev/Arduino/Shields/W5500_datasheet_v1.0.2_1.pdf
*/

uint8_t tx_cmd[3] = {0x00, 0x0F, 0x04};
uint8_t sendData[4] = {192, 168, 0, 2};
uint8_t rx_cmd[3] = {0x00, 0x0F, 0x00};
uint8_t receiveData[4] = {0x00, 0x00, 0x00, 0x00};

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SPI_CS, OUTPUT);
  SPI.begin();

  digitalWrite(PIN_SPI_CS, HIGH);

  SPI.beginTransaction(SPISettings(1000000, SPI_MSBFIRST, SPI_MODE0));
  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(tx_cmd, sizeof(tx_cmd));
  SPI.transfer(sendData, sizeof(sendData));
  digitalWrite(PIN_SPI_CS, HIGH);

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(rx_cmd, sizeof(rx_cmd));
  SPI.transfer(receiveData, sizeof(receiveData));
  digitalWrite(PIN_SPI_CS, HIGH);

  for(int i = 3; i < sizeof(rx_cmd); i++) {
    Serial.print("Data ");
    Serial.print(i-2);
    Serial.print(" = ");
    Serial.println(rx_cmd[i]);
  }
}

void loop() {
}

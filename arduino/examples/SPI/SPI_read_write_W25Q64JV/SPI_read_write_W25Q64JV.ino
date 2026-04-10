/*
  Write and Read data in Block 0 Sector 0 of W25q64JV-SIQ memory (address 0x00000800)

  Method details :
  - writeEnable       : Sets the Write Enable Latch (WEL) bit in status register to 1
                        The WEL bit must be set prior to every Page Program, Quad Page Program, Sector Erase, Block Erase, Chip erase, Write Status Register and Erase/Program Security Registers instruction
  - writeDisable      : Sets the Write Enable Latch (WEL) bit in status register to 0
  - isFlashBusy       : Reads status register 1 and return the S0 bit (BUSY status)
  - waitForFlashIdle  : Waits the flash for idle after busy condition
  - eraseSector       : Sets all memory within a specified sector to the erased state of all 1s (0xFF)
  - writeFlash        : Writes the data into previously erased (FFh) memory locations
  - readFlash         : Reads the data from the specified flash memory address

  Code details :
  1. Perform eraseSector method
      * Run writeEnable method first
      * send the 0x20 instruction code followed by 24-bit (3-byte) memory address in the same buffer
      * Perform waitForFlashIdle method
  
  2. Perform readFlash method and store the input data in prev_read
      * Send the 0x03 instruction code followed by 24-bit (3-byte) memory address in the same buffer
      * Send 1 byte dummy data using rx_buffer for storing the input data inside the rx_buffer
      * The 16-byte prev_read value data will become 0xFF

  3. Perform writeFlash method
      * Run writeEnable method first
      * Send the 0x02 instruction code followed by 24-bit (3-byte) memory address in the same buffer
      * Send the 16-byte write_buffer data
      * Perform waitForFlashIdle method 
  
  4. Perform readFlash method and store the input data in read_buffer
      * Send the 0x03 instruction code followed by 24-bit (3-byte) memory address in the same buffer
      * Send 1 byte dummy data using rx_buffer for storing the input data inside the rx_buffer
      * The read_buffer value will be the same as the write_buffer value

  Reference : W25Q64JVSIQ Datasheet

  https://www.winbond.com/hq/new-online-purchasing-guide/?__locale=en&pLine=/product/code-storage-flash/qspi-nor/&pNo=W25Q64JV
*/

#define BUFFER_SIZE 16

void writeEnable() {

  uint8_t cmd[1] = {0x06};

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(cmd, 1);
  digitalWrite(PIN_SPI_CS, HIGH);
}

void writeDisable() {

  uint8_t cmd[1] = {0x04};

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(cmd, 1);
  digitalWrite(PIN_SPI_CS, HIGH);
}

int isFlashBusy() {

  uint8_t cmd[1] = {0x05};
  char status;

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(cmd, 1);
  status = SPI.transfer(0x00);
  digitalWrite(PIN_SPI_CS, HIGH);

  return status & 0x01;
}

void waitForFlashIdle() {
  while(isFlashBusy()) {
    delayMicroseconds(100);
  }
}

void eraseSector(uint32_t addr) {

  writeEnable();

  uint8_t cmd[4];
  cmd[0] = 0x20;
  cmd[1] = (addr >> 16) & 0xFF;
  cmd[2] = (addr >> 8) & 0xFF;
  cmd[3] = addr & 0xFF;

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(cmd, 4);
  digitalWrite(PIN_SPI_CS, HIGH);

  waitForFlashIdle();
}

void writeFlash(uint32_t addr, uint8_t* data, uint8_t size) {

  writeEnable();

  uint8_t cmd[4];

  cmd[0] = 0x02;
  cmd[1] = 0x00;
  cmd[2] = 0x00;
  cmd[3] = 0x00;

  cmd[1] = (addr >> 16) & 0xFF;
  cmd[2] = (addr >> 8) & 0xFF;
  cmd[3] = addr & 0xFF;

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(cmd, 4);

  for (int idx = 0; idx < size; idx++) {
    SPI.transfer(data + idx, 1);
  }

  digitalWrite(PIN_SPI_CS, HIGH);

  waitForFlashIdle();
}

uint8_t readFlash(uint32_t addr) {

  uint8_t cmd[4];
  uint8_t rx_buffer[1] = {0x00};

  cmd[0] = 0x03;
  cmd[1] = (addr >> 16) & 0xFF;
  cmd[2] = (addr >> 8) & 0xFF;
  cmd[3] = addr & 0xFF;

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(cmd, 4);
  SPI.transfer(rx_buffer, 1);
  digitalWrite(PIN_SPI_CS, HIGH);
  
  return rx_buffer[0];
}

void setup() {
  Serial.begin(115200);
  SPI.begin();

  pinMode(PIN_SPI_CS, OUTPUT);
  digitalWrite(PIN_SPI_CS, HIGH);

  Serial.println("-----Test Start-----");

  SPI.beginTransaction(SPISettings());

  uint8_t read_buffer[BUFFER_SIZE], prev_read[BUFFER_SIZE];
  uint8_t write_buffer[BUFFER_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  uint32_t read_start_addr = 0x00000800;
  uint32_t read_addr;

  for (int i = 0; i < BUFFER_SIZE; ++i) {
    read_buffer[i] = 0;
    prev_read[i] = 0;
  }

  Serial.println("Erasing Sector...");
  eraseSector(read_start_addr);

  Serial.println("Initializing writing buffer value...");

  for (int i = 0; i<BUFFER_SIZE; i++) {
    read_addr = 0x00000800 + i;
    prev_read[i] = readFlash(read_addr);
  }

  Serial.println("Writing new data to external flash...");

  writeFlash(read_start_addr, write_buffer, BUFFER_SIZE);

  Serial.println("Reading and storing updated data from external flash...");

  for (int i = 0; i < BUFFER_SIZE; i++) {
    read_addr = 0x00000800 + i;
    read_buffer[i] = readFlash(read_addr);
  }

  for(int i = 0; i<BUFFER_SIZE; i++) {
    read_addr = 0x00000800 + i;
    Serial.print("Address : 0x");
    Serial.print(read_addr, HEX);
    Serial.print("\t   Value before : ");
    Serial.print(prev_read[i], HEX);
    Serial.print(" \t Value after : ");
    Serial.println(read_buffer[i], HEX);
  }
}

void loop() {
}

/*
  Multiple data transfer using SPI

  This code will send multiple dummy data using SPI

  Note :
  * Be careful for using this in loop
    The return value will be stored in the same buffer and replaced the existing "send_data" value
    Buffer overflow will be happened in the second loop and so on

  * There are 2 options to resolve this
    1. Declare the "send_data" variable inside the loop (Make it become a local variable instead of a global variable)
    2. Try to combine this method with single data SPI transfer method or 16-bit data SPI transfer method
        Use single SPI transfer method multiple times for fixed data value, for example : SPI instruction code
*/

uint8_t send_data[4] = {0xFF, 0xFF, 0xFF, 0xFF}; /* Dummy data to send*/

void setup() {
  Serial.begin(115200);

  SPI.begin();
  SPI.beginTransaction(SPISettings());

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(send_data, sizeof(send_data));
  digitalWrite(PIN_SPI_CS, HIGH);
  SPI.endTransaction();

  Serial.print("Data terkirim");
}

void loop() {

}

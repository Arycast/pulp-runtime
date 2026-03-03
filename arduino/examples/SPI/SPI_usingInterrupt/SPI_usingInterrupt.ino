/*
  SPI Communication API
  : SPI.usingInterrupt() function

  Registers an interrupt with the SPI library to prevent conflicts when performing SPI transactions within an interrupt service routine (ISR)
  When an interrupt is registered using this function, SPI.beginTransaction() will automatically disable the specified interrupt before starting the transaction, and SPI.endTransaction() will re-enable it
  This prevents data corruption that could occur if an interrupt tries to use the SPI bus while a transaction is in progress

  This example code will send "0xAA" data using SPI Communication and prevent interrupt beetwen the SPI
  If interrupt is happened during SPI Communication in main loop, it will prevent the program to enter myISR
  If interrupt is happened and the SPI communication is finished, it will jump and execute myISR

  Note :
  * Try to comment and un-comment "SPI.usingInterrupt(digitalPinToInterrupt(BTN1));"
  if using "SPI.usingInterrupt(digitalPinToInterrupt(BTN1));", the code will enter myISR after the SPI Communication is finished in main loop and BTN1 is pressed (to prevent conflicts)
  if comment "SPI.usingInterrupt(digitalPinToInterrupt(BTN1));", the code will enter myISR "every" BTN1 is pressed (conflict happen beetwen main loop and myISR)
*/

void setup() {
  Serial.begin(115200);
  SPI.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BTN1, INPUT);

  /* Registers an interrupt (BTN1) */
  SPI.usingInterrupt(digitalPinToInterrupt(BTN1)); /* try to comment this line to get the different result */

  /* Attach the interrupt (BTN1) */
  /* Jump to myISR when BTN1 is pressed (RISING mode) */
  attachInterrupt(digitalPinToInterrupt(BTN1), myISR, RISING);
  Serial.println("System ready, Please press BTN1 (interrupt trigger)");
}

void loop() {
  SPI.beginTransaction(SPISettings());


  digitalWrite(PIN_SPI_CS, LOW);
  for(int i = 0; i < 1000; i++) {
    SPI.transfer(0xAA); 
  }
  digitalWrite(PIN_SPI_CS, HIGH);

  SPI.endTransaction();

  delay(10);
  digitalWrite(LED_BUILTIN, LOW);
}

void myISR() {
  SPI.beginTransaction(SPISettings());

  digitalWrite(PIN_SPI_CS, LOW);
  SPI.transfer(0xFF);
  digitalWrite(PIN_SPI_CS, HIGH);
  SPI.endTransaction();

  digitalWrite(LED_BUILTIN, HIGH);
}

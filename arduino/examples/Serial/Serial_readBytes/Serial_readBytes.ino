/*
  Serial Communication API
  : Serial.readBytes() function

  Reads characters from the serial port into a buffer
  The function terminates if the determined length has been read, or it times out
  This fucntion inherits from Stream class

  This example code code creates a 5-position buffer and stores the incoming serial characters in it
  This code also print the buffer value

  Example :
  if Serial input value is "Hello", it will print "Received: Hello"
*/

char data[6];  /* 5 bytes + null terminator */

void setup() {
  Serial.begin(115200);

  Serial.println("Send 5 characters:");
}

void loop() {
  if (Serial.available() >= 5) {
    int bytesRead = Serial.readBytes(data, 5);
    data[bytesRead] = '\0';  /* Null-terminate the string */

    Serial.print("Received: ");
    Serial.println(data);
  }
}
/* The example is taken from https://docs.arduino.cc/language-reference/en/functions/external-interrupts/detachInterrupt/*/
/* Change the led state by attaching interrupt in a button */

const byte ledPin = LED1;
const byte interruptPin = BTN1;  // input pin that the interruption will be attached to
volatile byte state = LOW ;  // variable that will be updated in the ISR

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT);
  
  /* You can try other mode besides CHANGE such as RISING and FALLING*/
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop() {
  digitalWrite(ledPin, state);
}

void blink() {
  state = !state;
}
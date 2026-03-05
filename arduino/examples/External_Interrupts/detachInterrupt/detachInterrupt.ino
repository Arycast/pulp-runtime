/* Detach BTN1 interrupt (to change LED1 state) by pressing BTN2*/

const byte ledPin = LED1;
const byte indicatorLed = LED2; 
const byte interruptPin = BTN1;  // input pin that the interruption will be attached to
const byte detachBtnPin = BTN2;  

volatile byte state = LOW; // variable that will be updated in the ISR

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(indicatorLed, OUTPUT);
  
  pinMode(interruptPin, INPUT);
  pinMode(detachBtnPin, INPUT); 
  
  digitalWrite(ledPin, state);
  digitalWrite(indicatorLed, LOW);
  
  // Attach an interrupt on BTN1
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop() {
  digitalWrite(ledPin, state);
  
  if (digitalRead(detachBtnPin) == HIGH) {
    // Later, you can detach the interrupt when you no longer need it
    detachInterrupt(digitalPinToInterrupt(interruptPin));

    digitalWrite(indicatorLed, HIGH);
  }
}

void blink() {
  // This function will be executed when the interrupt on BTN1 is triggered
  // and it's still attached.  After detachInterrupt(), this routine won't be called.  
  state = !state;
}
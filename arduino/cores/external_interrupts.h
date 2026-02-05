void attachInterrupt(int interrupt, void(*userFunc)(void), int mode);
void detachInterrupt(int interrupt);
int digitalPinToInterrupt(int pin);
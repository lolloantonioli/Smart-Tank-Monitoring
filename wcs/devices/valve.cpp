#include "valve.h"

Valve::Valve(int p) : pin(p) {
  servo = new Servo();
  servo->attach(pin);
}

void Valve::setPosition(int percentage) {
  int angle = map(percentage, 0, 100, 0, 90);
  servo->write(angle);
}
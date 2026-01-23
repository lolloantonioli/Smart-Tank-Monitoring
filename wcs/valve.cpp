#include "valve.h"

Valve::Valve(int p) : pin(p) {
  servo = new Servo();
}
#include "potentiometer.h"

class Potentiometer::Potentiometer(int p) : pin(p) {}

void Potentiometer::init() {
    pinMode(pin, INPUT);
}
#include "potentiometer.h"

Potentiometer::Potentiometer(int p) : pin(p) {
    pinMode(pin, INPUT);
}

int Potentiometer::getValue() {
    return analogRead(pin);
}
#include "led.h"

Led::Led(int p) : pin(p) {
    pinMode(pin, OUTPUT);
    off();
}

void Led::on() {
    digitalWrite(pin, HIGH);
}

void Led::off() {
    digitalWrite(pin, LOW);
}

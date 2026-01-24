#include "button.h"

Button::Button(int p) : pin(p) {
    pinMode(pin, INPUT);
}

bool Button::isPressed() {
    return digitalRead(pin) == HIGH;
}
#include "manualInputTask.h"
#include <Arduino.h>
#include "../devices/potentiometer.h"

ManualInputTask::ManualInputTask(Button* btn, Potentiometer* p, Context* ctx) {
    button = btn;
    pot = p;
    context = ctx;
    lastButtonState = false;
}

void ManualInputTask::tick() {
    // Gestione Pulsante
    bool currState = button->isPressed();
    if (currState && !lastButtonState) {
        context->setButtonPressed(true);
    }
    lastButtonState = currState;

    // Gestione Potenziometro
    int val = pot->getValue();
    int level = map(val, 0, 1023, 0, 100);
    context->setManualLevel(level);
}
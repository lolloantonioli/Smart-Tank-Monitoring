#include "manualInputTask.h"
#include <Arduino.h>
#include "../devices/potentiometer.h"

ManualInputTask::ManualInputTask(Button* btn, Potentiometer* p, Context* ctx) {
    button = btn;
    pot = p;
    context = ctx;
    lastButtonState = false;
    lastPotVal = -1; 
}

void ManualInputTask::tick() {
    bool currState = button->isPressed();
    if (currState && !lastButtonState) {
        context->setButtonPressed(true);
    }
    lastButtonState = currState;

    int rawSum = 0;
    for(int i=0; i<3; i++) {
        rawSum += pot->getValue();
    }
    int val = rawSum / 3;
    
    int level = map(val, 0, 1023, 0, 100);
    
    if (abs(level - lastPotVal) >= 5) {
        context->setManualLevel(level);
        lastPotVal = level;
        
        Serial.print("VALVE:");
        Serial.println(level);
    }
}

#include "ledTask.h"

LedTask::LedTask(Led* green, Led* red, volatile TMSState* s) 
    : greenLed(green), redLed(red), stateRef(s) {}

void LedTask::tick() {
    switch (*stateRef) {
        case WORKING:
            greenLed->on();
            redLed->off();
            break;
        case NOT_WORKING:
            greenLed->off();
            redLed->on();
            break;
    }
}
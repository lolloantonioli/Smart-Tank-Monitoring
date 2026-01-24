#include "ledTask.h"

LedTask::LedTask(Led* green, Led* red, Context* ctx) 
    : greenLed(green), redLed(red), context(ctx) {}

void LedTask::tick() {
    TMSState state = context->getSystemState();

    switch (state) {
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
#include "displayTask.h"
#include "../constants.h"

DisplayTask::DisplayTask(Display* d, Context* c) : display(d), context(c) {}

void DisplayTask::tick() {
    WCSState state = context->getSystemState();
    int level = context->getValveLevel();
    
    const char* strState;
    switch (state) {
        case AUTOMATIC: strState = "AUTOMATIC"; break;
        case MANUAL:    strState = "MANUAL"; break;
        case UNCONNECTED: strState = "UNCONNECTED"; break;
        default: strState = "ERROR"; break;
    }
    display->clear();
    display->showStatus(strState, level);
}

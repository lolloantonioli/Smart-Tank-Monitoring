#include "logicTask.h"
#include "../constants.h"

LogicTask::LogicTask(Context* ctx) {
    context = ctx;
}

void LogicTask::tick() {
    if (context->fetchButtonPressed()) {
        if (!context->isRemoteUnconnected()) {
            WCSState curr = context->getSystemState();
            if (curr == MANUAL) {
                context->setSystemState(AUTOMATIC);
            } else {
                context->setSystemState(MANUAL);
            }
        }
    }

    if (context->isRemoteUnconnected()) {
        context->setSystemState(UNCONNECTED);
    }

    WCSState currentState = context->getSystemState();
    int targetValveLevel = 0;

    switch (currentState) {
        case MANUAL:
            targetValveLevel = context->getManualLevel();
            break;
            
        case AUTOMATIC:
            targetValveLevel = context->getRemoteAutoLevel();
            break;
            
        case UNCONNECTED:
            targetValveLevel = 0; 
            break;
    }

    context->setValveLevel(targetValveLevel);
}

#include "logicTask.h"

LogicTask::LogicTask(Context* ctx) : context(ctx) {}

void LogicTask::tick() {
    // Logica FSM per determinare lo stato del sistema
    bool wifi = context->isWifiConnected();
    bool mqtt = context->isMqttConnected();

    // Requisito: Green Led ON solo se network e data sending OK
    if (wifi && mqtt) {
        context->setSystemState(WORKING);
    } else {
        context->setSystemState(NOT_WORKING);
    }
}
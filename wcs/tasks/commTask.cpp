#include "commTask.h"

CommTask::CommTask(Context* ctx) {
    context = ctx;
    lastSentState = UNCONNECTED;
}

void CommTask::tick() {
    if (Serial.available() > 0) {
        String msg = Serial.readStringUntil('\n');
        msg.trim();
        if (msg.startsWith("VALVE:")) {
            int val = msg.substring(6).toInt();
            context->setRemoteAutoLevel(val);
        }
    }
    WCSState currentState = context->getSystemState();
    if (currentState != lastSentState) {
        if (currentState == MANUAL) {
            Serial.println("MOD:MANUAL");
        } else if (currentState == AUTOMATIC) {
            Serial.println("MOD:AUTO");
        }
        lastSentState = currentState;
    }
}
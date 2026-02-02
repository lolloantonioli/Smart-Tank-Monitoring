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
            if (context->getSystemState() == MANUAL) {
                context->setManualLevel(val);
            } else {
                context->setRemoteAutoLevel(val);
            }
            
        } else if (msg.equals("CMD:MANUAL")) {
            context->setSystemState(MANUAL);
        } else if (msg.equals("CMD:AUTO")) {
            context->setSystemState(AUTOMATIC);
        } else if (msg.equals("CMD:UNCONNECTED")) {
            context->setSystemState(UNCONNECTED);
        }
    }
    
    WCSState currentState = context->getSystemState();
    if (currentState != lastSentState) {
        if (currentState == MANUAL) {
            Serial.println("MOD:MANUAL");
        }
        else if (currentState == AUTOMATIC) {
            Serial.println("MOD:AUTO");
        }
        else if (currentState == UNCONNECTED) {
            Serial.println("MOD:UNCONNECTED");
        }
        lastSentState = currentState;
    }
}

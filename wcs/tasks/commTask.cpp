#include "commTask.h"

CommTask::CommTask(Context* ctx) {
    context = ctx;
}

void CommTask::tick() {
    if (Serial.available() > 0) {
        String msg = Serial.readStringUntil('\n');
        msg.trim();
        
        // Protocollo basato sugli stati dell'assignment:
        // "UNCONNECTED" -> Sistema disconnesso (TMS perso)
        // "AUTOMATIC"   -> Ripristino o forzatura Automatico
        // "MANUAL"      -> Forzatura Manuale (es. da Dashboard)
        // "LEVEL:xx"    -> Valore livello acqua per logica auto

        if (msg.equals("UNCONNECTED")) {
            context->setRemoteUnconnected(true);
        } 
        else if (msg.equals("AUTOMATIC")) {
            context->setRemoteUnconnected(false);
            context->setSystemState(AUTOMATIC);
        } 
        else if (msg.equals("MANUAL")) {
            context->setRemoteUnconnected(false);
            context->setSystemState(MANUAL);
        }
        else if (msg.startsWith("LEVEL:")) {
            int val = msg.substring(6).toInt();
            context->setRemoteAutoLevel(val);
        }
    }
}
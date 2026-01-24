#include "logicTask.h"
#include "../constants.h"

LogicTask::LogicTask(Context* ctx) {
    context = ctx;
}

void LogicTask::tick() {
    // 1. Gestione Pulsante Locale (Toggle)
    // Se premuto, inverte lo stato tra AUTOMATIC e MANUAL (se non siamo in UNCONNECTED)
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

    // 2. Controllo Priorità UNCONNECTED
    // Se il flag è alto (da CommTask), lo stato DEVE essere UNCONNECTED
    if (context->isRemoteUnconnected()) {
        context->setSystemState(UNCONNECTED);
    }

    // 3. Esecuzione Logica in base allo Stato Corrente
    WCSState currentState = context->getSystemState();
    int targetValveLevel = 0;

    switch (currentState) {
        case MANUAL:
            // In manuale il livello è dato dal potenziometro
            targetValveLevel = context->getManualLevel();
            break;
            
        case AUTOMATIC:
            // In automatico il livello arriva dal CUS (RemoteAutoLevel)
            targetValveLevel = context->getRemoteAutoLevel();
            break;
            
        case UNCONNECTED:
            // In disconnesso chiudiamo la valvola per sicurezza
            targetValveLevel = 0; 
            break;
    }

    // 4. Attuazione sul Context (ValveTask leggerà questo valore)
    context->setValveLevel(targetValveLevel);
}
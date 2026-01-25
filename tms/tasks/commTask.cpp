#include "commTask.h"

CommTask::CommTask(QueueHandle_t q, volatile TMSState* s) 
    : distanceQueue(q), stateRef(s) {
    client.setClient(espClient);
    client.setServer(mqtt_server, 1883);
}

void CommTask::checkConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        *stateRef = NOT_WORKING; 
        WiFi.begin(ssid, password);
    } else {
        if (!client.connected()) {
             *stateRef = NOT_WORKING;
             String clientId = "TMS-" + String(random(0xffff), HEX);
             if (client.connect(clientId.c_str())) {
                 *stateRef = WORKING;
             }
        } else {
            *stateRef = WORKING;
        }
    }
}

void CommTask::tick() {
    checkConnection();
    // Se siamo connessi, inviamo i dati
    if (*stateRef) {
        client.loop(); 
        float level;
        xQueuePeek(distanceQueue, &level, 0);
        char msg[20];
        snprintf(msg, 20, "%.2f", level);
        client.publish(topic, msg);
    }
}
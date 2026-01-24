#include "commTask.h"

CommTask::CommTask(Context* ctx) : context(ctx) {
    client.setClient(espClient);
    client.setServer(mqtt_server, 1883);
}

void CommTask::reconnect() {
    // Tentativo di connessione WiFi se necessario
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        // Non facciamo while() bloccante qui, controlliamo al prossimo tick
        // o facciamo un tentativo rapido. Per semplicità, controlliamo stato.
    }
    
    // Aggiorna stato WiFi nel context
    bool wifiStatus = (WiFi.status() == WL_CONNECTED);
    context->setWifiConnected(wifiStatus);

    if (wifiStatus && !client.connected()) {
        // Tenta connessione MQTT
        if (client.connect("TMS_Client")) {
            // Connesso
        } 
    }
}

void CommTask::tick() {
    // 1. Gestione Connessione
    reconnect();
    
    bool mqttStatus = client.connected();
    context->setMqttConnected(mqttStatus);
    
    if (mqttStatus) {
        client.loop(); // Keep-alive MQTT
        
        // 2. Invio Dati
        // Inviamo solo se connessi.
        float level = context->getDistance();
        
        // Formattiamo il payload. Esempio semplice stringa numerica o JSON
        char msg[20];
        snprintf(msg, 20, "%.2f", level);
        
        client.publish(topic, msg);
    }
}
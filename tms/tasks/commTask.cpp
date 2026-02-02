#include "commTask.h"

CommTask::CommTask(QueueHandle_t q, volatile TMSState* s) 
    : distanceQueue(q), stateRef(s) {
    
    // --- 1. AVVIO WIFI ---
    Serial.println("--- CommTask Avviato ---");
    Serial.print("Connessione WiFi a: ");
    Serial.println(ssid);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    client.setClient(espClient);
    client.setServer(mqtt_server, 1883);
}

void CommTask::checkConnection() {
    // --- 2. CONTROLLO WIFI ---
    if (WiFi.status() != WL_CONNECTED) {
        static unsigned long lastPrint = 0;
        if (millis() - lastPrint > 1000) {
            lastPrint = millis();
            Serial.print("WiFi Status: ");
            Serial.println(WiFi.status()); 
            Serial.println("WiFi: In attesa di connessione...");
        }
        *stateRef = NOT_WORKING; 
        
    } else {
        // --- 3. CONTROLLO MQTT ---
        if (!client.connected()) {
            Serial.println("WiFi: OK -> Tento connessione MQTT...");
            
            *stateRef = NOT_WORKING;
            String clientId = "TMS-" + String(random(0xffff), HEX);
            
            if (client.connect(clientId.c_str())) {
                Serial.println("MQTT: CONNESSO CON SUCCESSO!");
                *stateRef = WORKING;
            } else {
                Serial.print("MQTT Errore, rc=");
                Serial.println(client.state()); 
            }
        } else {
            *stateRef = WORKING;
        }
    }
}

void CommTask::tick() {
    checkConnection();

    if (*stateRef == WORKING) {
        client.loop(); 
        
        float level = 0.0;
        
        if (xQueueReceive(distanceQueue, &level, 0) == pdTRUE) {
            
            char msg[50];
            snprintf(msg, 50, "%.2f", level);
            
            Serial.print(">> INVIO MQTT: ");
            Serial.println(msg);
            
            client.publish(topic, msg);
        }
    }
    
    delay(10);
}
#include "commTask.h"

CommTask::CommTask(QueueHandle_t q, volatile TMSState* s) 
    : distanceQueue(q), stateRef(s) {
    
    // --- 1. AVVIO WIFI (Fallo solo qui nel costruttore!) ---
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
        // Stampiamo solo ogni tanto per non intasare la seriale
        static unsigned long lastPrint = 0;
        if (millis() - lastPrint > 1000) {
            lastPrint = millis();
            Serial.print("WiFi Status: ");
            Serial.println(WiFi.status()); // 3 = WL_CONNECTED, 6 = DISCONNECTED
            Serial.println("WiFi: In attesa di connessione...");
        }
        *stateRef = NOT_WORKING; 
        
        // NON chiamare WiFi.begin() qui! L'ESP32 si riconnette da solo.
    } else {
        // --- 3. CONTROLLO MQTT (Solo se WiFi è OK) ---
        if (!client.connected()) {
            Serial.println("WiFi: OK -> Tento connessione MQTT...");
            
            *stateRef = NOT_WORKING;
            String clientId = "TMS-" + String(random(0xffff), HEX);
            
            if (client.connect(clientId.c_str())) {
                Serial.println("MQTT: CONNESSO CON SUCCESSO!");
                *stateRef = WORKING;
            } else {
                Serial.print("MQTT Errore, rc=");
                Serial.println(client.state()); // Codici errore MQTT (es. -2 = server non trovato)
            }
        } else {
            // Tutto connesso
            *stateRef = WORKING;
        }
    }
}

void CommTask::tick() {
    checkConnection();

    // Se siamo connessi, inviamo i dati
    if (*stateRef == WORKING) {
        client.loop(); 
        
        float level = 0.0;
        
        // Usa xQueueReceive per togliere il dato dalla coda (evita spam)
        if (xQueueReceive(distanceQueue, &level, 0) == pdTRUE) {
            
            // Formatta in JSON per il Java: {"level": 50.00}
            char msg[50];
            snprintf(msg, 50, "%.2f", level);
            
            Serial.print(">> INVIO MQTT: ");
            Serial.println(msg);
            
            client.publish(topic, msg);
        }
    }
    
    // Piccolo delay per non bloccare il Watchdog (importante!)
    delay(10);
}
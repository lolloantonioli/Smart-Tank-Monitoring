#ifndef __COMM_TASK__
#define __COMM_TASK__

#include <WiFi.h>
#include <PubSubClient.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include "../kernel/task.h"
#include "../constants.h"

class CommTask : public Task {
  private:
    QueueHandle_t distanceQueue;  // Da leggere
    volatile TMSState* stateRef;  // Da scrivere
    
    WiFiClient espClient;
    PubSubClient client;
    
    const char* ssid = "YOUR_SSID";
    const char* password = "YOUR_PASS";
    const char* mqtt_server = "BROKER_IP"; 
    const char* topic = "tms/waterlevel";

    void checkConnection();

  public:
    CommTask(QueueHandle_t q, volatile TMSState* s);
    void tick();
};

#endif
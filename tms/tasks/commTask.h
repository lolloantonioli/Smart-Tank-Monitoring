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
    
    const char* ssid = "FASTWEB-ZCFLUU";
    const char* password = "LC4KN4PZS5";
    const char* mqtt_server = "broker.mqtt-dashboard.com"; 
    const char* topic = "tms/waterlevel";

    void checkConnection();

  public:
    CommTask(QueueHandle_t q, volatile TMSState* s);
    void tick();
};

#endif
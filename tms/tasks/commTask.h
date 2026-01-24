#ifndef __COMM_TASK__
#define __COMM_TASK__

#include <WiFi.h>
#include <PubSubClient.h>
#include "../kernel/task.h"
#include "../context.h"

class CommTask : public Task {
  private:
    Context* context;
    WiFiClient espClient;
    PubSubClient client;
    
    const char* ssid = "YOUR_WIFI_SSID";
    const char* password = "YOUR_WIFI_PASS";
    const char* mqtt_server = "BROKER_IP_ADDRESS"; // Es. 192.168.1.10
    const char* topic = "tms/waterlevel";

    void reconnect();

  public:
    CommTask(Context* ctx);
    void tick();
};

#endif
#ifndef __CONTEXT__
#define __CONTEXT__

#include "constants.h"

class Context {
  private:
    float currentDistance;
    bool wifiConnected;
    bool mqttConnected;
    TMSState systemState;

  public:
    Context();
    
    void setDistance(float dist);
    float getDistance();

    void setWifiConnected(bool status);
    bool isWifiConnected();

    void setMqttConnected(bool status);
    bool isMqttConnected();

    void setSystemState(TMSState state);
    TMSState getSystemState();
};

#endif
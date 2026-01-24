#include "context.h"

Context::Context() {
    currentDistance = 0.0;
    wifiConnected = false;
    mqttConnected = false;
    systemState = NOT_WORKING; // Default iniziale
}

void Context::setDistance(float dist) { currentDistance = dist; }
float Context::getDistance() { return currentDistance; }

void Context::setWifiConnected(bool status) { wifiConnected = status; }
bool Context::isWifiConnected() { return wifiConnected; }

void Context::setMqttConnected(bool status) { mqttConnected = status; }
bool Context::isMqttConnected() { return mqttConnected; }

void Context::setSystemState(TMSState state) { systemState = state; }
TMSState Context::getSystemState() { return systemState; }
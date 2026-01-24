#include "context.h"

Context::Context() {
  manualLevel = 0;
  buttonPressed = false;
  remoteAutoLevel = 0;
  remoteUnconnected = false;
  systemState = AUTOMATIC;
  valveLevel = 0;
}

void Context::setManualLevel(int level) { 
  if (level < 0) {
    level = 0;
  } 
  if (level > 100) {
    level = 100;
  }
  this->manualLevel = level; 
}

int Context::getManualLevel() { 
  return manualLevel; 
}

void Context::setButtonPressed(bool pressed) {
  this->buttonPressed = pressed;
}

bool Context::isButtonPressed() {
  return buttonPressed;
}

bool Context::fetchButtonPressed() {
  bool temp = buttonPressed;
  buttonPressed = false;
  return temp;
}

void Context::setRemoteAutoLevel(int level) { 
  // Qui potremmo anche aggiungere controlli di range
  this->remoteAutoLevel = level; 
}

int Context::getRemoteAutoLevel() {
  return remoteAutoLevel;
}

void Context::setRemoteUnconnected(bool status) {
  this->remoteUnconnected = status;
}
bool Context::isRemoteUnconnected() {
  return remoteUnconnected;
}

void Context::setSystemState(WCSState state) {
  this->systemState = state;
}

WCSState Context::getSystemState() {
  return systemState;
}

void Context::setValveLevel(int level) {
  this->valveLevel = level;
}

int Context::getValveLevel() {
  return valveLevel;
}

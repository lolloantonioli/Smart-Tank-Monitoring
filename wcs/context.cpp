#include "context.h"

Context::Context() {
  manualLevel = 0;
  buttonPressed = false;
  remoteAutoLevel = 0;
  remoteUnconnected = false;
  systemState = AUTOMATIC;
  valveLevel = 0;
}

void setManualLevel(int level) { 
  if (level < 0) {
    level = 0;
  } 
  if (level > 100) {
    level = 100;
  }
  this->manualLevel = level; 
}

int getManualLevel() { 
  return manualLevel; 
}

void setButtonPressed(bool pressed) {
  this->buttonPressed = pressed;
}

bool isButtonPressed() {
  return buttonPressed;
}

bool fetchButtonPressed() {
  bool temp = buttonPressed;
  buttonPressed = false;
  return temp;
}

void setRemoteAutoLevel(int level) { 
  // Qui potremmo anche aggiungere controlli di range
  this->remoteAutoLevel = level; 
}

int getRemoteAutoLevel() {
  return remoteAutoLevel;
}

void setRemoteUnconnected(bool status) {
  this->remoteUnconnected = status;
}
bool isRemoteUnconnected() {
  return remoteUnconnected;
}

void setSystemState(WCSState state) {
  this->systemState = state;
}

WCSState getSystemState() {
  return systemState;
}

void setValveLevel(int level) {
  this->valveLevel = level;
}

int getValveLevel() {
  return valveLevel;
}

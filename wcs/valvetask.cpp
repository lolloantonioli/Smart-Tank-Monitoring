#include "valveTask.h"
#include <Arduino.h>

ValveTask::ValveTask(Valve* valve, Context* context) {
  this->valve = valve;
  this->context = context;
}

void ValveTask::tick() {
  
}
#include "constants.h"

LogicTask::LogicTask() {
  currentState = AUTOMATIC;
}

void LogicTask::tick() {
  switch (currentState) {
    case AUTOMATIC:
      
      break;
    case MANUAL:
      break;
    case DISCONNECTED:
      break;
  }
}


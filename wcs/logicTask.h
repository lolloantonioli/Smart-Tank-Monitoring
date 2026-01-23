#ifndef __LOGIC_TASK__
#define __LOGIC_TASK__

#include "constants.h"

class LogicTask : public Task {
  private:
    State currentState;
    Display* display;
    Button* button;
  public:
    LogicTask();
    void tick();
}

#endif
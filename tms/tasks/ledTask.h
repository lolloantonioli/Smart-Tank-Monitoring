#ifndef __LED_TASK__
#define __LED_TASK__

#include "../kernel/task.h"
#include "../devices/led.h"
#include "../constants.h"

class LedTask : public Task {
  private:
    Led* greenLed;
    Led* redLed;
    volatile TMSState* stateRef;
  public:
    LedTask(Led* green, Led* red, volatile TMSState* s);
    void tick();
};

#endif
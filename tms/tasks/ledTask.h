#ifndef __LED_TASK__
#define __LED_TASK__

#include "../kernel/task.h"
#include "../devices/led.h"
#include "../context.h"

class LedTask : public Task {
  private:
    Led* greenLed;
    Led* redLed;
    Context* context;
  public:
    LedTask(Led* green, Led* red, Context* ctx);
    void tick();
};

#endif
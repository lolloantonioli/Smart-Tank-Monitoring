#ifndef __MANUAL_INPUT_TASK__
#define __MANUAL_INPUT_TASK__

#include "../kernel/task.h"
#include "../devices/button.h"
#include "../devices/potentiometer.h"
#include "../context.h"

class ManualInputTask : public Task {
  private:
    Button* button;
    Potentiometer* pot;
    Context* context;
    bool lastButtonState;
    int lastPotVal;
  public:
    ManualInputTask(Button* btn, Potentiometer* p, Context* ctx);
    void tick();
};


#endif

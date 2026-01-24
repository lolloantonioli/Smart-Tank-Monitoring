#ifndef __DISPLAY_TASK__
#define __DISPLAY_TASK__

#include "../kernel/task.h"
#include "../devices/display.h"
#include "../context.h"

class DisplayTask : public Task {
  private:
    Display* display;
    Context* context;
  public:
    DisplayTask();
    void tick();
}

#endif
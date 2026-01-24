#ifndef __VALVE_TASK__
#define __VALVE_TASK__

#include "../kernel/task.h"
#include "../devices/valve.h"
#include "../context.h"

class ValveTask : public Task {
  private:
    Valve* valve;
    Context* context;
  public:
    ValveTask(Valve* valve, Context* context);
    void tick();
}

#endif
#ifndef __LOGIC_TASK__
#define __LOGIC_TASK__

#include "../kernel/task.h"
#include "../context.h"
#include "../constants.h"

class LogicTask : public Task {
  private:
    Context* context;
  public:
    LogicTask(Context* ctx);
    void tick();
};

#endif
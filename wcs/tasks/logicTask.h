#ifndef __LOGIC_TASK__
#define __LOGIC_TASK__

#include "../context.h"
#include "../kernel/task.h"

class LogicTask : public Task {
  private:
    Context* context;
  public:
    LogicTask(Context* ctx);
    void tick();
};

#endif
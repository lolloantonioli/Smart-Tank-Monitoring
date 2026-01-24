#ifndef __COMM_TASK__
#define __COMM_TASK__

#include "../kernel/task.h"
#include "../context.h"
#include <Arduino.h>

class CommTask : public Task {
  private:
    Context* context;
  public:
    CommTask(Context* ctx);
    void tick();
}

#endif
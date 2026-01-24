#ifndef __SONAR_TASK__
#define __SONAR_TASK__

#include "../kernel/task.h"
#include "../devices/sonar.h"
#include "../context.h"

class SonarTask : public Task {
  private:
    Sonar* sonar;
    Context* context;
  public:
    SonarTask(Sonar* s, Context* c);
    void tick();
};

#endif
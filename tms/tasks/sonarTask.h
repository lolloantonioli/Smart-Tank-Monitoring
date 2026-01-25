#ifndef __SONAR_TASK__
#define __SONAR_TASK__

#include "../kernel/task.h"
#include "../devices/sonar.h"
#include <Arduino.h>

class SonarTask : public Task {
  private:
    Sonar* sonar;
    volatile float* distRef;
  public:
    SonarTask(Sonar* s, volatile float* c);
    void tick();
};

#endif
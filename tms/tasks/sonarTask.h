#ifndef __SONAR_TASK__
#define __SONAR_TASK__

#include "../kernel/task.h"
#include "../devices/sonar.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

class SonarTask : public Task {
  private:
    Sonar* sonar;
    QueueHandle_t distanceQueue;
  public:
    SonarTask(Sonar* s, QueueHandle_t distanceQueue);
    void tick();
};

#endif
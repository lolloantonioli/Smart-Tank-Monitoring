#include "sonarTask.h"

SonarTask::SonarTask(Sonar* s, QueueHandle_t q) 
    : sonar(s), distanceQueue(q) {}

void SonarTask::tick() {
    float distance = sonar->getDistanceCM();
    xQueueOverwrite(distanceQueue, &distance);
}
#include "sonarTask.h"

SonarTask::SonarTask(Sonar* s, Context* c) : sonar(s), context(c) {}

void SonarTask::tick() {
    float dist = sonar->getDistanceCM();
    context->setDistance(dist);
}
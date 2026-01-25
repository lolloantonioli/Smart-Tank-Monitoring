#include "sonarTask.h"

SonarTask::SonarTask(Sonar* s, volatile float* d) : sonar(s), distRef(d) {}

void SonarTask::tick() {
    *distRef = sonar->getDistanceCM();
}
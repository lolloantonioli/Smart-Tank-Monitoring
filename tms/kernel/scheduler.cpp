#include "scheduler.h"
#include <Arduino.h>

void Scheduler::init(int basePeriod){
  this->basePeriod = basePeriod;
  nTasks = 0;
}

bool Scheduler::addTask(Task* task){
  if (nTasks < MAX_TASKS-1){
    taskList[nTasks] = task;
    nTasks++;
    return true;
  } else {
    return false; 
  }
}
  
void Scheduler::schedule(){     
  static unsigned long lastTime = 0;
  if (millis() - lastTime >= basePeriod) {
      lastTime = millis();
      
      for (int i = 0; i < nTasks; i++){
        if (taskList[i]->isActive()){
          if (taskList[i]->isPeriodic()){
            if (taskList[i]->updateAndCheckTime(basePeriod)){
              taskList[i]->tick();
            }
          } else {
            taskList[i]->tick();
            if (taskList[i]->isCompleted()){
              taskList[i]->setActive(false);
            }
          }
        }
      }
  }
}
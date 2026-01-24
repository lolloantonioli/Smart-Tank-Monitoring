#ifndef __SONAR__
#define __SONAR__

#include <Arduino.h>

class Sonar {
  private:
    int trigPin, echoPin;
    float vs; 
  public:
    Sonar(int trig, int echo);
    float getDistanceCM();
};

#endif
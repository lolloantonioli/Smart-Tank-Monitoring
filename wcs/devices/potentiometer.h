#ifndef __POTENTIOMETER__
#define __POTENTIOMETER__

#include <Arduino.h>

class Potentiometer {
  private:
    int pin;
  public:
    Potentiometer(int p);
    int getValue();
};

#endif
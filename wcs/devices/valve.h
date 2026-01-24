#ifndef __VALVE__
#define __VALVE__

#include <Servo.h>
#include <Arduino.h>

class Valve {
  private:
    Servo* servo;
    int pin;
  public:
    Valve(int p);
    void setPosition(int percentage);
};

#endif
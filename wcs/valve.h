#ifndef __VALVE__
#define __VALVE__

#include <Servo.h>

class Valve {
  private:
    Servo* servo;
    int pin;
  public:
    Valve(int p);
}

#endif
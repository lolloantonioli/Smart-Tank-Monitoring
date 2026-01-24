#ifndef __LED__
#define __LED__

#include <Arduino.h>

class Led {
  private:
    int pin;
  public:
    Led(int p);
    void on();
    void off();
};

#endif
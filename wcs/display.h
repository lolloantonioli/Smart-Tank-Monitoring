#ifndef __DISPLAY__
#define __DISPLAY__

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class Display {
  private:
    LiquidCrystal_I2C* lcd;
  public:
    Display(uint8_t addr, uint8_t cols, uint8_t rows);
    
    void init();
    void clear();
};

#endif
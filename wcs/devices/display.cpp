#include "display.h"
#include <stdio.h>

Display::Display(uint8_t addr, uint8_t cols, uint8_t rows) {
    lcd = new LiquidCrystal_I2C(addr, cols, rows);
    lcd->init();
    lcd->backlight();
    clear();
}

void Display::clear() {
    lcd->clear();
}

void Display::showStatus(const char* state, int level) {
    lcd->setCursor(0, 0);
    lcd->print("Mode:           ");
    lcd->setCursor(6, 0);
    lcd->print(state);
    
    lcd->setCursor(0, 1);
    char buff[16];
    sprintf(buff, "Valve: %d%%     ", level);
    lcd->print(buff);
}
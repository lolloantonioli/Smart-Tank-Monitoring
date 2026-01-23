#include "display.h"

Display::Display(uint8_t addr, uint8_t cols, uint8_t rows) {
    lcd = new LiquidCrystal_I2C(addr, cols, rows);
}

void Display::init() {
    lcd->init();
    lcd->backlight();
    clear();
}

void Display::clear() {
    lcd->clear();
}
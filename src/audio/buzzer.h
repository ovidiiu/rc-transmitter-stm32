#pragma once
#include <Arduino.h>
#include "../config.h"

class Buzzer {
public:
    void begin();
    void beep(uint16_t freqHz, uint16_t durationMs);
    void startup();
    void warning();
    void error();
    void keypress();
    void update();

private:
    uint32_t _endTime = 0;
    bool _active = false;
};

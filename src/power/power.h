#pragma once
#include <Arduino.h>
#include "../config.h"

class Power {
public:
    void begin();
    void update();
    bool isShuttingDown();

private:
    uint32_t _btnPressStart = 0;
    bool     _btnWasPressed = false;
    bool     _shuttingDown = false;
};

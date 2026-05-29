#include "power.h"

void Power::begin() {
    // Latch power immediately
    pinMode(PIN_POWER_HOLD, OUTPUT);
    digitalWrite(PIN_POWER_HOLD, HIGH);

    // Read power button state
    pinMode(PIN_POWER_SENSE, INPUT_PULLUP);

    _btnPressStart = 0;
    _btnWasPressed = false;
    _shuttingDown = false;
}

void Power::update() {
    if (_shuttingDown) return;

    bool pressed = !digitalRead(PIN_POWER_SENSE);  // active low

    if (pressed && !_btnWasPressed) {
        // Button just pressed
        _btnPressStart = millis();
    }

    if (pressed && _btnWasPressed) {
        // Button held — check duration
        if (millis() - _btnPressStart >= POWER_OFF_HOLD_MS) {
            _shuttingDown = true;
        }
    }

    if (!pressed) {
        _btnPressStart = 0;
    }

    _btnWasPressed = pressed;
}

bool Power::isShuttingDown() {
    return _shuttingDown;
}

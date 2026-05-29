#include "switches.h"

void Switches::begin() {
    for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
        pinMode(_btnPins[i], INPUT_PULLUP);
    }
    for (uint8_t i = 0; i < NUM_TRIMS; i++) {
        pinMode(_trimPlus[i], INPUT_PULLUP);
        pinMode(_trimMinus[i], INPUT_PULLUP);
    }

    pinMode(PIN_SW_A, INPUT_PULLUP);
    pinMode(PIN_SW_B_UP, INPUT_PULLUP);
    pinMode(PIN_SW_B_DN, INPUT_PULLUP);
    pinMode(PIN_SW_C_UP, INPUT_PULLUP);
    pinMode(PIN_SW_C_DN, INPUT_PULLUP);
    pinMode(PIN_SW_D, INPUT_PULLUP);
    pinMode(PIN_SW_F, INPUT_PULLUP);
    pinMode(PIN_SW_H, INPUT_PULLUP);
}

void Switches::scan() {
    uint32_t now = millis();

    for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
        uint8_t raw = !digitalRead(_btnPins[i]);
        if (raw != _btnState[i] && (now - _btnLastChange[i]) > INPUT_DEBOUNCE_MS) {
            _btnPrev[i] = _btnState[i];
            _btnState[i] = raw;
            _btnLastChange[i] = now;
        }
    }

    swA = getSwitch2(PIN_SW_A);
    swB = getSwitch3(PIN_SW_B_UP, PIN_SW_B_DN);
    swC = getSwitch3(PIN_SW_C_UP, PIN_SW_C_DN);
    swD = getSwitch2(PIN_SW_D);
    swF = getSwitch2(PIN_SW_F);
    swH = getSwitch2(PIN_SW_H);

    for (uint8_t i = 0; i < NUM_TRIMS; i++) {
        if (!digitalRead(_trimPlus[i]) && (now - _trimLastPress[i]) > 150) {
            trimVal[i] = constrain(trimVal[i] + TRIM_STEP, TRIM_MIN, TRIM_MAX);
            _trimLastPress[i] = now;
        }
        if (!digitalRead(_trimMinus[i]) && (now - _trimLastPress[i]) > 150) {
            trimVal[i] = constrain(trimVal[i] - TRIM_STEP, TRIM_MIN, TRIM_MAX);
            _trimLastPress[i] = now;
        }
    }
}

bool Switches::button(uint8_t idx) {
    return (idx < NUM_BUTTONS) ? _btnState[idx] : false;
}

bool Switches::buttonPressed(uint8_t idx) {
    if (idx >= NUM_BUTTONS) return false;
    bool pressed = (_btnState[idx] && !_btnPrev[idx]);
    _btnPrev[idx] = _btnState[idx];
    return pressed;
}

SwitchPos Switches::getSwitch2(uint8_t pin) {
    return digitalRead(pin) ? SW_UP : SW_DOWN;
}

SwitchPos Switches::getSwitch3(uint8_t pinUp, uint8_t pinDn) {
    if (!digitalRead(pinUp)) return SW_UP;
    if (!digitalRead(pinDn)) return SW_DOWN;
    return SW_MID;
}

int8_t Switches::getTrim(uint8_t trimIdx) {
    return (trimIdx < NUM_TRIMS) ? trimVal[trimIdx] : 0;
}

void Switches::resetTrims() {
    for (uint8_t i = 0; i < NUM_TRIMS; i++) trimVal[i] = 0;
}

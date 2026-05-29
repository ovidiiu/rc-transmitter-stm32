#pragma once
#include <Arduino.h>
#include "../config.h"

enum SwitchPos : int8_t {
    SW_UP   = -1,
    SW_MID  =  0,
    SW_DOWN =  1
};

class Switches {
public:
    void begin();
    void scan();

    bool button(uint8_t idx);
    bool buttonPressed(uint8_t idx);

    SwitchPos getSwitch2(uint8_t pin);
    SwitchPos getSwitch3(uint8_t pinUp, uint8_t pinDn);

    int8_t getTrim(uint8_t trimIdx);
    void resetTrims();

    SwitchPos swA, swB, swC, swD, swF, swH;
    int8_t trimVal[NUM_TRIMS] = {0};

private:
    static const uint8_t NUM_BUTTONS = 7;
    const uint8_t _btnPins[NUM_BUTTONS] = {
        PIN_BTN_MENU, PIN_BTN_PAGE, PIN_BTN_EXIT, PIN_BTN_ENTER,
        PIN_ENC_A, PIN_ENC_B, PIN_ENC_BTN
    };

    uint8_t  _btnState[NUM_BUTTONS] = {0};
    uint8_t  _btnPrev[NUM_BUTTONS] = {0};
    uint32_t _btnLastChange[NUM_BUTTONS] = {0};

    const uint8_t _trimPlus[NUM_TRIMS]  = {
        PIN_TRIM1_PLUS, PIN_TRIM2_PLUS, PIN_TRIM3_PLUS, PIN_TRIM4_PLUS
    };
    const uint8_t _trimMinus[NUM_TRIMS] = {
        PIN_TRIM1_MINUS, PIN_TRIM2_MINUS, PIN_TRIM3_MINUS, PIN_TRIM4_MINUS
    };
    uint32_t _trimLastPress[NUM_TRIMS] = {0};
};

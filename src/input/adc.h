#pragma once
#include <Arduino.h>
#include "../config.h"

struct StickCalibration {
    uint16_t min;
    uint16_t mid;
    uint16_t max;
};

class ADCInput {
public:
    void begin();
    void read();
    void calibrate(uint8_t stick, uint16_t min, uint16_t mid, uint16_t max);

    int16_t  getChannel(uint8_t ch);
    uint16_t getCRSF(uint8_t ch);
    float    getBatteryVoltage();
    uint16_t getRaw(uint8_t ch);

    StickCalibration cal[NUM_STICKS + NUM_POTS];

private:
    const uint8_t _pins[NUM_ANALOGS] = {
        PIN_STICK_AIL, PIN_STICK_ELE, PIN_STICK_THR, PIN_STICK_RUD,
        PIN_POT_VRA, PIN_POT_VRB, PIN_VBAT
    };

    uint32_t _accumulator[NUM_ANALOGS] = {0};
    uint16_t _raw[NUM_ANALOGS] = {0};
    uint8_t  _sampleCount = 0;

    int16_t applyCalibration(uint8_t ch, uint16_t raw);
};

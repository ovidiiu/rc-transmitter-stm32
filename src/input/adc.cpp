#include "adc.h"

void ADCInput::begin() {
    analogReadResolution(ADC_RESOLUTION);

    for (uint8_t i = 0; i < NUM_ANALOGS; i++) {
        pinMode(_pins[i], INPUT_ANALOG);
    }

    for (uint8_t i = 0; i < NUM_STICKS + NUM_POTS; i++) {
        cal[i].min = 0;
        cal[i].mid = ADC_MAX / 2;
        cal[i].max = ADC_MAX;
    }
}

void ADCInput::read() {
    for (uint8_t i = 0; i < NUM_ANALOGS; i++) {
        _accumulator[i] += analogRead(_pins[i]);
    }
    _sampleCount++;

    if (_sampleCount >= ADC_OVERSAMPLE) {
        for (uint8_t i = 0; i < NUM_ANALOGS; i++) {
            _raw[i] = _accumulator[i] / ADC_OVERSAMPLE;
            _accumulator[i] = 0;
        }
        _sampleCount = 0;
    }
}

uint16_t ADCInput::getRaw(uint8_t ch) {
    return (ch < NUM_ANALOGS) ? _raw[ch] : 0;
}

int16_t ADCInput::applyCalibration(uint8_t ch, uint16_t raw) {
    if (ch >= NUM_STICKS + NUM_POTS) return 0;

    int32_t val;
    if (raw <= cal[ch].mid) {
        val = map((int32_t)raw, cal[ch].min, cal[ch].mid, -1024, 0);
    } else {
        val = map((int32_t)raw, cal[ch].mid, cal[ch].max, 0, 1024);
    }
    return constrain(val, -1024, 1024);
}

int16_t ADCInput::getChannel(uint8_t ch) {
    if (ch >= NUM_STICKS + NUM_POTS) return 0;
    return applyCalibration(ch, _raw[ch]);
}

uint16_t ADCInput::getCRSF(uint8_t ch) {
    return map(getChannel(ch), -1024, 1024, CRSF_CHANNEL_MIN, CRSF_CHANNEL_MAX);
}

void ADCInput::calibrate(uint8_t stick, uint16_t min, uint16_t mid, uint16_t max) {
    if (stick >= NUM_STICKS + NUM_POTS) return;
    cal[stick] = {min, mid, max};
}

float ADCInput::getBatteryVoltage() {
    return (_raw[NUM_STICKS + NUM_POTS] / (float)ADC_MAX) * 3.3f * VBAT_SCALE;
}

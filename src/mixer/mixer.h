#pragma once
#include <Arduino.h>
#include "../config.h"

enum MixSource : uint8_t {
    SRC_NONE = 0,
    SRC_AIL, SRC_ELE, SRC_THR, SRC_RUD,
    SRC_VRA, SRC_VRB,
    SRC_SW_A, SRC_SW_B, SRC_SW_C, SRC_SW_D, SRC_SW_F, SRC_SW_H,
    SRC_TRIM1, SRC_TRIM2, SRC_TRIM3, SRC_TRIM4,
    SRC_MAX
};

struct MixEntry {
    MixSource source;
    int8_t    weight;
    int8_t    offset;
    bool      enabled;
};

struct ModelData {
    char      name[MODEL_NAME_LEN + 1];
    MixEntry  mixes[CRSF_NUM_CHANNELS][MAX_MIXES_PER_CH];
    int8_t    trimValues[NUM_TRIMS];
    uint8_t   rfRate;
    uint8_t   rfPower;
};

class Mixer {
public:
    void begin();
    void setInput(uint8_t ch, int16_t value);
    void setSwitchInput(MixSource sw, int16_t value);
    void run();

    uint16_t getOutputCRSF(uint8_t ch);
    int16_t  getOutput(uint8_t ch);

    ModelData model;

private:
    int16_t _inputs[20] = {0};
    int16_t _outputs[CRSF_NUM_CHANNELS] = {0};
};

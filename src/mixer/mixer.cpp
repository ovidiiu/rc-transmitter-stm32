#include "mixer.h"

void Mixer::begin() {
    memset(&model, 0, sizeof(model));
    strncpy(model.name, "Default", MODEL_NAME_LEN);

    for (uint8_t ch = 0; ch < 4; ch++) {
        model.mixes[ch][0].source  = (MixSource)(SRC_AIL + ch);
        model.mixes[ch][0].weight  = 100;
        model.mixes[ch][0].offset  = 0;
        model.mixes[ch][0].enabled = true;
    }

    model.mixes[4][0] = {SRC_VRA, 100, 0, true};
    model.mixes[5][0] = {SRC_VRB, 100, 0, true};
}

void Mixer::setInput(uint8_t ch, int16_t value) {
    if (ch < 20) _inputs[ch] = value;
}

void Mixer::setSwitchInput(MixSource sw, int16_t value) {
    if (sw < 20) _inputs[sw] = value;
}

void Mixer::run() {
    for (uint8_t ch = 0; ch < CRSF_NUM_CHANNELS; ch++) {
        int32_t sum = 0;
        bool active = false;

        for (uint8_t m = 0; m < MAX_MIXES_PER_CH; m++) {
            MixEntry &mix = model.mixes[ch][m];
            if (!mix.enabled || mix.source == SRC_NONE) continue;

            int32_t val = (mix.source == SRC_MAX) ? 1024 : _inputs[mix.source];
            val = (val * mix.weight) / 100;
            val += (mix.offset * 1024) / 100;
            sum += val;
            active = true;
        }

        _outputs[ch] = active ? constrain(sum, -1024, 1024) : 0;
    }
}

int16_t Mixer::getOutput(uint8_t ch) {
    return (ch < CRSF_NUM_CHANNELS) ? _outputs[ch] : 0;
}

uint16_t Mixer::getOutputCRSF(uint8_t ch) {
    return map(getOutput(ch), -1024, 1024, CRSF_CHANNEL_MIN, CRSF_CHANNEL_MAX);
}

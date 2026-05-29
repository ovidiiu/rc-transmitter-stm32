#pragma once
#include <Arduino.h>
#include "../config.h"

#define CRSF_SYNC_BYTE          0xC8
#define CRSF_FRAMETYPE_RC       0x16
#define CRSF_FRAMETYPE_LINK     0x14
#define CRSF_FRAMETYPE_BATTERY  0x08
#define CRSF_CHANNEL_MIN        172
#define CRSF_CHANNEL_MID        992
#define CRSF_CHANNEL_MAX        1811

struct CrsfTelemetry {
    uint8_t  rssi_up;
    uint8_t  rssi_down;
    uint8_t  link_quality;
    int8_t   snr;
    uint16_t voltage;
    uint16_t current;
    uint32_t capacity;
    uint8_t  remaining;
};

class CRSF {
public:
    void begin(HardwareSerial &serial);
    void sendChannels(uint16_t channels[CRSF_NUM_CHANNELS]);
    bool parseTelemetry();

    CrsfTelemetry telemetry;

private:
    HardwareSerial *_serial = nullptr;
    uint8_t _txBuf[CRSF_MAX_FRAME];
    uint8_t _rxBuf[CRSF_MAX_FRAME];
    uint8_t _rxIdx = 0;

    uint8_t crc8_dvb_s2(uint8_t crc, uint8_t data);
    uint8_t calcCRC(uint8_t *buf, uint8_t len);
    void packChannels(uint16_t channels[CRSF_NUM_CHANNELS], uint8_t *payload);
};

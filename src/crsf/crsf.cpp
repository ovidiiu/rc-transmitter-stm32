#include "crsf.h"

void CRSF::begin(HardwareSerial &serial) {
    _serial = &serial;
    _serial->begin(CRSF_BAUDRATE);
}

uint8_t CRSF::crc8_dvb_s2(uint8_t crc, uint8_t data) {
    crc ^= data;
    for (uint8_t i = 0; i < 8; i++) {
        if (crc & 0x80)
            crc = (crc << 1) ^ 0xD5;
        else
            crc <<= 1;
    }
    return crc;
}

uint8_t CRSF::calcCRC(uint8_t *buf, uint8_t len) {
    uint8_t crc = 0;
    for (uint8_t i = 0; i < len; i++) {
        crc = crc8_dvb_s2(crc, buf[i]);
    }
    return crc;
}

void CRSF::packChannels(uint16_t ch[], uint8_t *p) {
    p[0]  = (uint8_t)(ch[0] & 0xFF);
    p[1]  = (uint8_t)((ch[0] >> 8) | (ch[1] << 3));
    p[2]  = (uint8_t)((ch[1] >> 5) | (ch[2] << 6));
    p[3]  = (uint8_t)((ch[2] >> 2) & 0xFF);
    p[4]  = (uint8_t)((ch[2] >> 10) | (ch[3] << 1));
    p[5]  = (uint8_t)((ch[3] >> 7) | (ch[4] << 4));
    p[6]  = (uint8_t)((ch[4] >> 4) | (ch[5] << 7));
    p[7]  = (uint8_t)((ch[5] >> 1) & 0xFF);
    p[8]  = (uint8_t)((ch[5] >> 9) | (ch[6] << 2));
    p[9]  = (uint8_t)((ch[6] >> 6) | (ch[7] << 5));
    p[10] = (uint8_t)((ch[7] >> 3));
    p[11] = (uint8_t)(ch[8] & 0xFF);
    p[12] = (uint8_t)((ch[8] >> 8) | (ch[9] << 3));
    p[13] = (uint8_t)((ch[9] >> 5) | (ch[10] << 6));
    p[14] = (uint8_t)((ch[10] >> 2) & 0xFF);
    p[15] = (uint8_t)((ch[10] >> 10) | (ch[11] << 1));
    p[16] = (uint8_t)((ch[11] >> 7) | (ch[12] << 4));
    p[17] = (uint8_t)((ch[12] >> 4) | (ch[13] << 7));
    p[18] = (uint8_t)((ch[13] >> 1) & 0xFF);
    p[19] = (uint8_t)((ch[13] >> 9) | (ch[14] << 2));
    p[20] = (uint8_t)((ch[14] >> 6) | (ch[15] << 5));
    p[21] = (uint8_t)((ch[15] >> 3));
}

void CRSF::sendChannels(uint16_t channels[CRSF_NUM_CHANNELS]) {
    if (!_serial) return;

    _txBuf[0] = CRSF_SYNC_BYTE;
    _txBuf[1] = 24;
    _txBuf[2] = CRSF_FRAMETYPE_RC;
    packChannels(channels, &_txBuf[3]);
    _txBuf[25] = calcCRC(&_txBuf[2], 23);

    _serial->write(_txBuf, 26);
}

bool CRSF::parseTelemetry() {
    if (!_serial) return false;

    while (_serial->available()) {
        uint8_t b = _serial->read();

        if (_rxIdx == 0 && b != CRSF_SYNC_BYTE) continue;
        _rxBuf[_rxIdx++] = b;

        if (_rxIdx == 2 && _rxBuf[1] > CRSF_MAX_FRAME - 2) {
            _rxIdx = 0;
            continue;
        }

        if (_rxIdx >= 2 && _rxIdx == _rxBuf[1] + 2) {
            uint8_t len = _rxBuf[1];
            uint8_t crc = calcCRC(&_rxBuf[2], len - 1);

            if (crc == _rxBuf[len + 1]) {
                uint8_t type = _rxBuf[2];

                if (type == CRSF_FRAMETYPE_LINK) {
                    telemetry.rssi_up      = _rxBuf[3];
                    telemetry.rssi_down    = _rxBuf[4];
                    telemetry.link_quality = _rxBuf[5];
                    telemetry.snr          = (int8_t)_rxBuf[6];
                }
                else if (type == CRSF_FRAMETYPE_BATTERY) {
                    telemetry.voltage   = (_rxBuf[3] << 8) | _rxBuf[4];
                    telemetry.current   = (_rxBuf[5] << 8) | _rxBuf[6];
                    telemetry.capacity  = (_rxBuf[7] << 16) | (_rxBuf[8] << 8) | _rxBuf[9];
                    telemetry.remaining = _rxBuf[10];
                }
            }
            _rxIdx = 0;
        }

        if (_rxIdx >= CRSF_MAX_FRAME) _rxIdx = 0;
    }
    return true;
}

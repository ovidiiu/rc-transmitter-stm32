#pragma once
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "../config.h"

class Display {
public:
    void begin();
    void clear();
    void flush();

    void drawText(uint8_t x, uint8_t y, const char *text);
    void drawTextSmall(uint8_t x, uint8_t y, const char *text);
    void drawTextLarge(uint8_t x, uint8_t y, const char *text);
    void drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
    void drawFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
    void drawHBar(uint8_t x, uint8_t y, uint8_t w, uint8_t h,
                  int16_t value, int16_t minVal, int16_t maxVal);

    void drawHeader(const char *title);
    void drawBattery(uint8_t x, uint8_t y, float voltage);
    void drawChannelBars(int16_t channels[], uint8_t count);
    void drawStickBox(uint8_t x, uint8_t y, int16_t hVal, int16_t vVal);

    U8G2_SH1107_128X128_F_HW_I2C u8g2 = U8G2_SH1107_128X128_F_HW_I2C(U8G2_R0);

private:
    void setDisplayOffset(uint8_t offset);
};

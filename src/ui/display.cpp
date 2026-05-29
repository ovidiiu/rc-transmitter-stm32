#include "display.h"

void Display::setDisplayOffset(uint8_t offset) {
    u8x8_t *u8x8 = u8g2.getU8x8();
    u8x8_cad_StartTransfer(u8x8);
    u8x8_cad_SendCmd(u8x8, 0xD3);
    u8x8_cad_SendArg(u8x8, offset);
    u8x8_cad_EndTransfer(u8x8);
}

void Display::begin() {
    Wire.setSDA(PIN_I2C_SDA);
    Wire.setSCL(PIN_I2C_SCL);
    Wire.begin();
    Wire.setClock(400000);

    u8g2.begin();
    setDisplayOffset(DISPLAY_OFFSET);
    u8g2.setContrast(180);
}

void Display::clear()  { u8g2.clearBuffer(); }
void Display::flush()  { u8g2.sendBuffer();  }

void Display::drawText(uint8_t x, uint8_t y, const char *text) {
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(x, y, text);
}

void Display::drawTextSmall(uint8_t x, uint8_t y, const char *text) {
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.drawStr(x, y, text);
}

void Display::drawTextLarge(uint8_t x, uint8_t y, const char *text) {
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(x, y, text);
}

void Display::drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h)   { u8g2.drawBox(x, y, w, h);   }
void Display::drawFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h) { u8g2.drawFrame(x, y, w, h); }

void Display::drawHeader(const char *title) {
    u8g2.drawBox(0, 0, 128, 12);
    u8g2.setDrawColor(0);
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(2, 10, title);
    u8g2.setDrawColor(1);
}

void Display::drawHBar(uint8_t x, uint8_t y, uint8_t w, uint8_t h,
                        int16_t value, int16_t minVal, int16_t maxVal) {
    u8g2.drawFrame(x, y, w, h);
    int16_t fill = map(value, minVal, maxVal, 0, w - 2);
    fill = constrain(fill, 0, w - 2);
    u8g2.drawBox(x + 1, y + 1, fill, h - 2);
}

void Display::drawBattery(uint8_t x, uint8_t y, float voltage) {
    char buf[8];
    dtostrf(voltage, 4, 1, buf);
    strcat(buf, "V");
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.drawStr(x, y, buf);

    uint8_t ix = x + 30;
    u8g2.drawFrame(ix, y - 7, 16, 8);
    u8g2.drawBox(ix + 16, y - 5, 2, 4);
    int8_t bars = constrain(map(voltage * 10, 66, 84, 0, 12), 0, 12);
    u8g2.drawBox(ix + 2, y - 5, bars, 4);
}

void Display::drawChannelBars(int16_t channels[], uint8_t count) {
    uint8_t barH = 6;
    uint8_t startY = 16;
    uint8_t num = min(count, (uint8_t)8);
    uint8_t spacing = (128 - startY) / num;

    for (uint8_t i = 0; i < num; i++) {
        uint8_t y = startY + i * spacing;
        char label[4];
        snprintf(label, sizeof(label), "%d", i + 1);
        u8g2.setFont(u8g2_font_5x7_tr);
        u8g2.drawStr(0, y + barH, label);
        drawHBar(12, y, 100, barH, channels[i], -1024, 1024);
        u8g2.drawPixel(12 + 50, y);
        u8g2.drawPixel(12 + 50, y + barH - 1);
    }
}

void Display::drawStickBox(uint8_t x, uint8_t y, int16_t hVal, int16_t vVal) {
    const uint8_t sz = 40;
    u8g2.drawFrame(x, y, sz, sz);
    u8g2.drawHLine(x, y + sz / 2, sz);
    u8g2.drawVLine(x + sz / 2, y, sz);
    uint8_t dx = map(hVal,  -1024, 1024, 2, sz - 3);
    uint8_t dy = map(-vVal, -1024, 1024, 2, sz - 3);
    u8g2.drawDisc(x + dx, y + dy, 2);
}

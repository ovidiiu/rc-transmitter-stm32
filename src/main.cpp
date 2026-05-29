#include <Arduino.h>
#include "config.h"
#include "input/adc.h"
#include "input/switches.h"
#include "mixer/mixer.h"
#include "crsf/crsf.h"
#include "ui/display.h"
#include "audio/buzzer.h"
#include "power/power.h"

// ---- Global Objects ----
ADCInput   adc;
Switches   switches;
Mixer      mixer;
CRSF       crsf;
Display    display;
Buzzer     buzzer;

HardwareSerial CRSFSerial(PIN_CRSF_RX, PIN_CRSF_TX);

// ---- Timing ----
uint32_t lastLoop    = 0;
uint32_t lastCRSF    = 0;
uint32_t lastDisplay = 0;
uint32_t lastTelem   = 0;


Power      power;

// ---- UI ----
enum Page : uint8_t {
    PAGE_MAIN = 0,
    PAGE_CHANNELS,
    PAGE_MIXER_SETUP,
    PAGE_MODEL,
    PAGE_INPUTS_CAL,
    PAGE_TELEMETRY,
    PAGE_SYSTEM,
    PAGE_COUNT
};

Page currentPage = PAGE_MAIN;
bool lowBattWarned = false;

// ---- Forward declarations ----
void drawPageMain();
void drawPageChannels();
void drawPageTelemetry();
void drawPageSystem();
void handleNavigation();

// ========================================
void setup() {
        // FIRST: latch power on
    power.begin();

    pinMode(PIN_MODULE_EN, OUTPUT);
    digitalWrite(PIN_MODULE_EN, LOW);

    display.begin();
    adc.begin();
    switches.begin();
    mixer.begin();
    buzzer.begin();

    // Splash screen
    display.clear();
    display.drawTextLarge(20, 50, "RC TX v1.0");
    display.drawText(22, 75, "STM32F412RET6");
    display.drawText(28, 95, "ELRS / CRSF");
    display.flush();
    buzzer.startup();
    delay(1500);

    // Power on ELRS module
    digitalWrite(PIN_MODULE_EN, HIGH);
    delay(100);
    crsf.begin(CRSFSerial);
}

// ========================================
void loop() {
    uint32_t now = millis();

    // 1ms: Read inputs
    adc.read();
    switches.scan();

    // Feed inputs to mixer
    for (uint8_t i = 0; i < NUM_STICKS + NUM_POTS; i++) {
        mixer.setInput(i + 1, adc.getChannel(i));
    }

    mixer.setSwitchInput(SRC_SW_A, switches.swA * 1024);
    mixer.setSwitchInput(SRC_SW_B, switches.swB * 1024);
    mixer.setSwitchInput(SRC_SW_C, switches.swC * 1024);
    mixer.setSwitchInput(SRC_SW_D, switches.swD * 1024);
    mixer.setSwitchInput(SRC_SW_F, switches.swF * 1024);
    mixer.setSwitchInput(SRC_SW_H, switches.swH * 1024);

    for (uint8_t i = 0; i < NUM_TRIMS; i++) {
        int16_t trimScaled = (int16_t)switches.getTrim(i) * 1024 / TRIM_MAX;
        mixer.setInput(SRC_TRIM1 + i, trimScaled);
    }

    mixer.run();

    // 4ms: Send CRSF
    if (now - lastCRSF >= CRSF_INTERVAL_MS) {
        lastCRSF = now;
        uint16_t crsfChannels[CRSF_NUM_CHANNELS];
        for (uint8_t i = 0; i < CRSF_NUM_CHANNELS; i++) {
            crsfChannels[i] = mixer.getOutputCRSF(i);
        }
        crsf.sendChannels(crsfChannels);
    }

    // 100ms: Telemetry
    if (now - lastTelem >= TELEM_INTERVAL_MS) {
        lastTelem = now;
        crsf.parseTelemetry();
    }

    // 20ms: Display
    if (now - lastDisplay >= DISPLAY_INTERVAL_MS) {
        lastDisplay = now;
        handleNavigation();

        display.clear();

        switch (currentPage) {
            case PAGE_MAIN:       drawPageMain();      break;
            case PAGE_CHANNELS:   drawPageChannels();  break;
            case PAGE_TELEMETRY:  drawPageTelemetry(); break;
            case PAGE_SYSTEM:     drawPageSystem();    break;
            default:
                display.drawHeader("TODO");
                display.drawText(10, 60, "Not implemented");
                break;
        }

        display.flush();
    }

    // Battery warning
    float vbat = adc.getBatteryVoltage();
    if (vbat < VBAT_LOW && !lowBattWarned) {
        buzzer.warning();
        lowBattWarned = true;
    }
    if (vbat >= VBAT_LOW + 0.2f) lowBattWarned = false;
    
    // Power button check
    power.update();

    if (power.isShuttingDown()) {
        // Save any settings here (future: SD card)

        // Show shutdown screen
        display.clear();
        display.drawTextLarge(20, 60, "Power Off...");
        display.flush();

        buzzer.beep(1000, 200);
        delay(300);
        buzzer.beep(500, 300);
        delay(500);

        // Kill power
        digitalWrite(PIN_MODULE_EN, LOW);   // turn off ELRS module
        digitalWrite(PIN_POWER_HOLD, LOW);  // release power latch

        // If still alive (USB powered), just halt
        while (1) { delay(1000); }
    }

    buzzer.update();


    // Pace main loop to 1ms
    while (micros() - lastLoop < LOOP_INTERVAL_US) { /* spin */ }
    lastLoop = micros();
}

// ========================================
void handleNavigation() {
    if (switches.buttonPressed(1)) {  // PAGE
        currentPage = (Page)((currentPage + 1) % PAGE_COUNT);
        buzzer.keypress();
    }
    if (switches.buttonPressed(2)) {  // EXIT
        currentPage = PAGE_MAIN;
        buzzer.keypress();
    }
}

// ========================================
void drawPageMain() {
    display.drawHeader("MAIN");
    display.drawBattery(75, 10, adc.getBatteryVoltage());
    display.drawTextLarge(2, 26, mixer.model.name);

    display.drawStickBox(4,  44, adc.getChannel(3), adc.getChannel(1));
    display.drawStickBox(74, 44, adc.getChannel(0), adc.getChannel(2));

    char buf[24];
    snprintf(buf, sizeof(buf), "T:%+3d %+3d %+3d %+3d",
        switches.trimVal[0], switches.trimVal[1],
        switches.trimVal[2], switches.trimVal[3]);
    display.drawTextSmall(4, 95, buf);

    snprintf(buf, sizeof(buf), "A%+d B%+d C%+d D%+d",
        switches.swA, switches.swB, switches.swC, switches.swD);
    display.drawTextSmall(4, 106, buf);

    snprintf(buf, sizeof(buf), "LQ:%d%%", crsf.telemetry.link_quality);
    display.drawTextSmall(4, 117, buf);

    snprintf(buf, sizeof(buf), "RSSI:%d", crsf.telemetry.rssi_up);
    display.drawTextSmall(64, 117, buf);
}

void drawPageChannels() {
    display.drawHeader("CHANNELS");
    int16_t chValues[8];
    for (uint8_t i = 0; i < 8; i++) chValues[i] = mixer.getOutput(i);
    display.drawChannelBars(chValues, 8);
}

void drawPageTelemetry() {
    display.drawHeader("TELEMETRY");
    char buf[24];

    snprintf(buf, sizeof(buf), "RSSI Up:  %d dBm", crsf.telemetry.rssi_up);
    display.drawText(4, 28, buf);

    snprintf(buf, sizeof(buf), "RSSI Dn:  %d dBm", crsf.telemetry.rssi_down);
    display.drawText(4, 42, buf);

    snprintf(buf, sizeof(buf), "LQ:       %d %%", crsf.telemetry.link_quality);
    display.drawText(4, 56, buf);

    snprintf(buf, sizeof(buf), "SNR:      %d dB", crsf.telemetry.snr);
    display.drawText(4, 70, buf);

    float rxV = crsf.telemetry.voltage / 10.0f;
    char vStr[8];
    dtostrf(rxV, 4, 1, vStr);
    snprintf(buf, sizeof(buf), "RX Batt:  %sV", vStr);
    display.drawText(4, 84, buf);

    snprintf(buf, sizeof(buf), "Capacity: %lumAh", crsf.telemetry.capacity);
    display.drawText(4, 98, buf);

    snprintf(buf, sizeof(buf), "Remain:   %d %%", crsf.telemetry.remaining);
    display.drawText(4, 112, buf);
}

void drawPageSystem() {
    display.drawHeader("SYSTEM");
    char buf[24];

    float v = adc.getBatteryVoltage();
    char vStr[8];
    dtostrf(v, 4, 1, vStr);
    snprintf(buf, sizeof(buf), "TX Batt: %sV", vStr);
    display.drawText(4, 28, buf);

    display.drawText(4, 44, "MCU: STM32F412RE");
    display.drawText(4, 58, "FW:  v1.0.0");
    display.drawText(4, 72, "CRSF: 420000 baud");

    snprintf(buf, sizeof(buf), "Model: %s", mixer.model.name);
    display.drawText(4, 86, buf);

    uint32_t sec = millis() / 1000;
    snprintf(buf, sizeof(buf), "Up: %02lu:%02lu:%02lu",
        sec / 3600, (sec / 60) % 60, sec % 60);
    display.drawText(4, 100, buf);
}

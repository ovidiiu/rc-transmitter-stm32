#include "buzzer.h"

void Buzzer::begin() {
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);
}

void Buzzer::beep(uint16_t freqHz, uint16_t durationMs) {
    tone(PIN_BUZZER, freqHz, durationMs);
    _endTime = millis() + durationMs;
    _active = true;
}

void Buzzer::update() {
    if (_active && millis() >= _endTime) {
        noTone(PIN_BUZZER);
        _active = false;
    }
}

void Buzzer::startup() {
    tone(PIN_BUZZER, 1000, 100); delay(120);
    tone(PIN_BUZZER, 1500, 100); delay(120);
    tone(PIN_BUZZER, 2000, 150);
}

void Buzzer::warning()  { beep(2500, 200); }
void Buzzer::error()    { beep(500, 500);  }
void Buzzer::keypress() { beep(3000, 30);  }

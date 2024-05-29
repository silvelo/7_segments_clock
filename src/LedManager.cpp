#include "LedManager.h"

LedManager &LedManager::getInstance() {
    static LedManager instance;
    return instance;
}

LedManager::LedManager()
    : hoursStrip(NUM_PIXELS, HOUR_PIN, NEO_GRB + NEO_KHZ800),
      minutesStrip(NUM_PIXELS, MINUTE_PIN, NEO_GRB + NEO_KHZ800),
      secondsStrip(SECOND_NUM_PIXELS, SECOND_PIN, NEO_GRB + NEO_KHZ800),
      preferencesManager(PreferencesManager::getInstance()) {
}

void LedManager::begin() {
    Serial.begin(115200);
    hoursStrip.begin();
    minutesStrip.begin();
    secondsStrip.begin();
    updateColorsFromPreferences();
    Serial.println("Led Manager inicializado.");
}

void LedManager::end() {
    hoursStrip.clear();
    minutesStrip.clear();
    secondsStrip.clear();

    hoursStrip.show();
    minutesStrip.show();
    secondsStrip.show();
}

void LedManager::showHours(int hour) {
    this->showHourDigit(hour % 10, 0, this->hourColor1);
    this->showHourDigit(hour / 10, NUM_PIXELS / 2, this->hourColor2);

    hoursStrip.show();
}

void LedManager::showMinutes(int minutes) {
    this->showMinuteDigit(minutes / 10, 0, this->minuteColor1);
    this->showMinuteDigit(minutes % 10, NUM_PIXELS / 2, this->minuteColor2);

    minutesStrip.show();
}

void LedManager::showColon() {
    secondsStrip.setPixelColor(0, this->dotsColor1);
    secondsStrip.setPixelColor(1, this->dotsColor2);
    secondsStrip.show();
}

void LedManager::updateColorsFromPreferences() {
    hourColor1 = preferencesManager.getHourLed1Color();
    hourColor2 = preferencesManager.getHourLed2Color();
    minuteColor1 = preferencesManager.getMinutesLed1Color();
    minuteColor2 = preferencesManager.getMinutesLed2Color();
    dotsColor1 = preferencesManager.getDotsLed1Color();
    dotsColor2 = preferencesManager.getDotsLed2Color();
}

void LedManager::showHourDigit(int number, int startPixel, uint32_t color) {
    for (int i = 0; i < 7; i++) {
        if (hourSegments[number][i]) {
            hoursStrip.setPixelColor(startPixel + (i * 2), color);
            hoursStrip.setPixelColor(startPixel + (i * 2) + 1, color);
        } else {
            hoursStrip.setPixelColor(startPixel + (i * 2), hoursStrip.Color(0, 0, 0));
            hoursStrip.setPixelColor(startPixel + (i * 2) + 1, hoursStrip.Color(0, 0, 0));
        }
    }
}

void LedManager::showMinuteDigit(int number, int startPixel, uint32_t color) {
    for (int i = 0; i < 7; i++) {
        if (minuteSegments[number][i]) {
            minutesStrip.setPixelColor(startPixel + (i * 2), color);
            minutesStrip.setPixelColor(startPixel + (i * 2) + 1, color);
        } else {
            minutesStrip.setPixelColor(startPixel + (i * 2), minutesStrip.Color(0, 0, 0));
            minutesStrip.setPixelColor(startPixel + (i * 2) + 1, minutesStrip.Color(0, 0, 0));
        }
    }
}

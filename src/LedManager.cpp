#include "LedManager.h"

LedManager &LedManager::getInstance() {
    static LedManager instance;
    return instance;
}

LedManager::LedManager()
    : hoursStrip(0, HOUR_PIN, NEO_GRB + NEO_KHZ800),
      minutesStrip(0, MINUTE_PIN, NEO_GRB + NEO_KHZ800),
      secondsStrip(0, SECOND_PIN, NEO_GRB + NEO_KHZ800),
      preferencesManager(PreferencesManager::getInstance()) {
}

void LedManager::begin() {
    Serial.begin(115200);
    hoursStrip.begin();
    minutesStrip.begin();
    secondsStrip.begin();
    updateSegmentsFromPreferences();
    updateColorsFromPreferences();
    Serial.println("Led Manager init...");
    hoursStrip.updateLength(hoursLength);
    minutesStrip.updateLength(minutesLength);
    secondsStrip.updateLength(secondsLength);
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
    this->showHourDigit(hour / 10, hoursLength / 2, this->hourColor2);

    hoursStrip.show();
}

void LedManager::showMinutes(int minutes) {
    this->showMinuteDigit(minutes / 10, 0, this->minuteColor1);
    this->showMinuteDigit(minutes % 10, minutesLength / 2, this->minuteColor2);

    minutesStrip.show();
}

void LedManager::showColon() {
    for (int i = 0; i < secondsLength / 2; i++) {
        secondsStrip.setPixelColor(i, this->dotsColor1);
        secondsStrip.setPixelColor(i + (secondsLength / 2), this->dotsColor2);
    }

    secondsStrip.show();
}

void LedManager::updateSegmentsFromPreferences() {
    ledsPerSegment = preferencesManager.getLedsPerSegment();
    hoursLength = HOUR_DIGITS * HOUR_SEGMENT * ledsPerSegment;
    minutesLength = MINUTE_DIGITS * MINUTE_SEGMENT * ledsPerSegment;
    secondsLength = SECOND_DIGITS * SECOND_SEGMENT * ledsPerSegment;
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
    for (int i = 0; i < HOUR_SEGMENT; i++) {
        if (hourSegments[number][i]) {
            for (int j = 0; j < ledsPerSegment; j++) {
                hoursStrip.setPixelColor(startPixel + (i * ledsPerSegment) + j, color);
            }
        } else {
            for (int j = 0; j < ledsPerSegment; j++) {
                hoursStrip.setPixelColor(startPixel + (i * ledsPerSegment) + j,
                                         hoursStrip.Color(0, 0, 0));
            }
        }
    }
}

void LedManager::showMinuteDigit(int number, int startPixel, uint32_t color) {
    Serial.println("Number " + String(number));
    for (int i = 0; i < MINUTE_SEGMENT; i++) {
        Serial.println("Segment number " + String(i));
        if (minuteSegments[number][i]) {
            Serial.println("Power On pixels:");
            for (int j = 0; j < ledsPerSegment; j++) {
                Serial.print("[");
                Serial.print(" Start Pixel: ");
                Serial.print(startPixel);
                Serial.print(" i: ");
                Serial.print(i);
                Serial.print(" ledsPerSegment: ");
                Serial.print(ledsPerSegment);
                Serial.print(" j: ");
                Serial.print(j);
                Serial.print("] - ");
                Serial.print(startPixel + (i * ledsPerSegment) + j);
                minutesStrip.setPixelColor(startPixel + (i * ledsPerSegment) + j, color);
                Serial.println();
            }
        } else {
            for (int j = 0; j < ledsPerSegment; j++) {
                minutesStrip.setPixelColor(startPixel + (i * ledsPerSegment) + j,
                                           minutesStrip.Color(0, 0, 0));
            }
        }
        Serial.println();
    }
}

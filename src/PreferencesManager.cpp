#include "PreferencesManager.h"

PreferencesManager &PreferencesManager::getInstance() {
    static PreferencesManager instance;
    return instance;
}

void PreferencesManager::begin() {
    Serial.begin(115200);

    if (!SPIFFS.begin()) {
        Serial.println("Error mounting file system");
        return;
    }

    preferences.begin(preferencesNamespace, false);
    Serial.println("Preferences init...");
}

int PreferencesManager::getTimeOffset() {
    return preferences.getInt(TIME_OFFSET);
}

void PreferencesManager::setTimeOffset(int timeOffset) {
    Serial.println("Set time offset: " + String(timeOffset) + " hours.");
    preferences.putInt(TIME_OFFSET, timeOffset);
}

String PreferencesManager::getStarHour() {
    Serial.println("Get start timestamp");
    return preferences.getString(START_HOUR);
}

void PreferencesManager::setStartHour(String startHour) {
    Serial.println("Set start timestamp: " + startHour);
    preferences.putString(START_HOUR, startHour);
}

String PreferencesManager::getEndHour() {
    Serial.println("Get end timestamp");
    return preferences.getString(END_HOUR);
}

void PreferencesManager::setEndHour(String EndHour) {
    Serial.println("Set end timestamp: " + EndHour);
    preferences.putString(END_HOUR, EndHour);
}

uint32_t PreferencesManager::getHourLed1Color() {
    return preferences.getUInt(HOUR_LED_1_COLOR, DEFAULT_LED_COLOR);
}

void PreferencesManager::setHourLed1Color(uint32_t color) {
    preferences.putUInt(HOUR_LED_1_COLOR, color);
}

uint32_t PreferencesManager::getHourLed2Color() {
    return preferences.getUInt(HOUR_LED_2_COLOR, DEFAULT_LED_COLOR);
}

void PreferencesManager::setHourLed2Color(uint32_t color) {
    preferences.putUInt(HOUR_LED_2_COLOR, color);
}

uint32_t PreferencesManager::getDotsLed1Color() {
    return preferences.getUInt(DOTS_LED_1_COLOR, DEFAULT_LED_COLOR);
}

void PreferencesManager::setDotsLed1Color(uint32_t color) {
    preferences.putUInt(DOTS_LED_1_COLOR, color);
}

uint32_t PreferencesManager::getDotsLed2Color() {
    return preferences.getUInt(DOTS_LED_2_COLOR, DEFAULT_LED_COLOR);
}

void PreferencesManager::setDotsLed2Color(uint32_t color) {
    preferences.putUInt(DOTS_LED_2_COLOR, color);
}

uint32_t PreferencesManager::getMinutesLed1Color() {
    return preferences.getUInt(MINUTES_LED_1_COLOR, DEFAULT_LED_COLOR);
}

void PreferencesManager::setMinutesLed1Color(uint32_t color) {
    preferences.putUInt(MINUTES_LED_1_COLOR, color);
}

uint32_t PreferencesManager::getMinutesLed2Color() {
    return preferences.getUInt(MINUTES_LED_2_COLOR, DEFAULT_LED_COLOR);
}

void PreferencesManager::setMinutesLed2Color(uint32_t color) {
    preferences.putUInt(MINUTES_LED_2_COLOR, color);
}

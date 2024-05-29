#include "PreferencesManager.h"

PreferencesManager &PreferencesManager::getInstance() {
    static PreferencesManager instance;
    return instance;
}

void PreferencesManager::begin() {
    Serial.begin(115200);

    if (!SPIFFS.begin()) {
        Serial.println("Error al montar el sistema de archivos");
        return;
    }

    preferences.begin(preferencesNamespace, false);
    Serial.println("Preferences inicializado.");
}

int PreferencesManager::getTimeOffset() {
    return preferences.getInt(TIME_OFFSET);
}

void PreferencesManager::setTimeOffset(int timeOffset) {
    Serial.println("Set time offset: " + String(timeOffset) + " hours.");
    preferences.putInt(TIME_OFFSET, timeOffset);
}

String PreferencesManager::getStartTimestamp() {
    Serial.println("Get start timestamp");
    return preferences.getString(START_TIMESTAMP);
}

void PreferencesManager::setStartTimestamp(String startTimestamp) {
    Serial.println("Set start timestamp: " + startTimestamp);
    preferences.putString(START_TIMESTAMP, startTimestamp);
}

String PreferencesManager::getEndTimestamp() {
    Serial.println("Get end timestamp");
    return preferences.getString(END_TIMESTAMP);
}

void PreferencesManager::setEndTimestamp(String endTimestamp) {
    Serial.println("Set end timestamp: " + endTimestamp);
    preferences.putString(END_TIMESTAMP, endTimestamp);
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

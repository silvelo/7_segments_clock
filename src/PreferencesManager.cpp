#include "PreferencesManager.h"

// Definir el valor predeterminado para los colores LED (blanco)
const uint32_t DEFAULT_LED_COLOR = 0xFFFFFF;

PreferencesManager &PreferencesManager::getInstance()
{
    static PreferencesManager instance;
    return instance;
}

void PreferencesManager::begin(const char *namespace_)
{
    preferences.begin(preferencesNamespace, false);
}

int PreferencesManager::getTimeOffset()
{
    return preferences.getInt("timeOffset", 0);
}

void PreferencesManager::setTimeOffset(int timeOffset)
{
    preferences.putInt("timeOffset", timeOffset);
}

uint32_t PreferencesManager::getStartTimestamp()
{
    return preferences.getUInt("startTimestamp", 0);
}

void PreferencesManager::setStartTimestamp(uint32_t startTimestamp)
{
    preferences.putUInt("startTimestamp", startTimestamp);
}

uint32_t PreferencesManager::getEndTimestamp()
{
    return preferences.getUInt("endTimestamp", 0);
}

void PreferencesManager::setEndTimestamp(uint32_t endTimestamp)
{
    preferences.putUInt("endTimestamp", endTimestamp);
}

uint32_t PreferencesManager::getHourLed1Color()
{
    return preferences.getUInt("hourLed1Color", DEFAULT_LED_COLOR);
}

void PreferencesManager::setHourLed1Color(uint32_t color)
{
    preferences.putUInt("hourLed1Color", color);
}

uint32_t PreferencesManager::getHourLed2Color()
{
    return preferences.getUInt("hourLed2Color", DEFAULT_LED_COLOR);
}

void PreferencesManager::setHourLed2Color(uint32_t color)
{
    preferences.putUInt("hourLed2Color", color);
}

uint32_t PreferencesManager::getDotsLed1Color()
{
    return preferences.getUInt("dotsLed1Color", DEFAULT_LED_COLOR);
}

void PreferencesManager::setDotsLed1Color(uint32_t color)
{
    preferences.putUInt("dotsLed1Color", color);
}

uint32_t PreferencesManager::getDotsLed2Color()
{
    return preferences.getUInt("dotsLed2Color", DEFAULT_LED_COLOR);
}

void PreferencesManager::setDotsLed2Color(uint32_t color)
{
    preferences.putUInt("dotsLed2Color", color);
}

uint32_t PreferencesManager::getMinutesLed1Color()
{
    return preferences.getUInt("minutesLed1Color", DEFAULT_LED_COLOR);
}

void PreferencesManager::setMinutesLed1Color(uint32_t color)
{
    preferences.putUInt("minutesLed1Color", color);
}

uint32_t PreferencesManager::getMinutesLed2Color()
{
    return preferences.getUInt("minutesLed2Color", DEFAULT_LED_COLOR);
}

void PreferencesManager::setMinutesLed2Color(uint32_t color)
{
    preferences.putUInt("minutesLed2Color", color);
}

#ifndef PREFERENCES_MANAGER_H
#define PREFERENCES_MANAGER_H

#include <Preferences.h>

class PreferencesManager
{
public:
    static PreferencesManager &getInstance();

    void begin(const char *namespace_);
    int getTimeOffset();
    void setTimeOffset(int timeOffset);
    uint32_t getStartTimestamp();
    void setStartTimestamp(uint32_t startTimestamp);
    uint32_t getEndTimestamp();
    void setEndTimestamp(uint32_t endTimestamp);
    uint32_t getHourLed1Color();
    void setHourLed1Color(uint32_t color);
    uint32_t getHourLed2Color();
    void setHourLed2Color(uint32_t color);
    uint32_t getDotsLed1Color();
    void setDotsLed1Color(uint32_t color);
    uint32_t getDotsLed2Color();
    void setDotsLed2Color(uint32_t color);
    uint32_t getMinutesLed1Color();
    void setMinutesLed1Color(uint32_t color);
    uint32_t getMinutesLed2Color();
    void setMinutesLed2Color(uint32_t color);

private:
    PreferencesManager() = default;
    PreferencesManager(const PreferencesManager &) = delete;
    PreferencesManager &operator=(const PreferencesManager &) = delete;

    const char *preferencesNamespace = "NeoPixelClock";
    Preferences preferences;
};

#endif

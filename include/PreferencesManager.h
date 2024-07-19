#ifndef PREFERENCES_MANAGER_H
#define PREFERENCES_MANAGER_H

#define DEFAULT_LED_COLOR 0xFFFFFF
#define DEFAULT_LEDS_PER_SEGMENT 2
#define TIME_OFFSET "to"
#define START_HOUR "st"
#define END_HOUR "et"
#define HOUR_LED_1_COLOR "hl1c"
#define HOUR_LED_2_COLOR "hl2c"
#define DOTS_LED_1_COLOR "dl1c"
#define DOTS_LED_2_COLOR "dl2c"
#define MINUTES_LED_1_COLOR "ml1c"
#define MINUTES_LED_2_COLOR "ml2c"
#define LEDS_PER_SEGMENT "lps"
#define DEBUG "debug"

#include <Preferences.h>
#include <SPIFFS.h>
class PreferencesManager {
   public:
    static PreferencesManager &getInstance();

    void begin();
    int getTimeOffset();
    void setTimeOffset(int timeOffset);
    String getStartTimestamp();
    void setStartHour(String startTimestamp);
    String getStarHour();
    void setEndHour(String endTimestamp);
    String getEndHour();
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
    int getLedsPerSegment();
    void setLedsPerSegment(int numLeds);
    bool getDebug();
    void setDebug(bool debug);

   private:
    PreferencesManager() = default;
    PreferencesManager(const PreferencesManager &) = delete;
    PreferencesManager &operator=(const PreferencesManager &) = delete;

    const char *preferencesNamespace = "NeoPixelClock";
    Preferences preferences;
};

#endif

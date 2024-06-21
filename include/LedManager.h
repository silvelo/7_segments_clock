#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Adafruit_NeoPixel.h>
#include "PreferencesManager.h"

#define MINUTE_PIN 16
#define SECOND_PIN 18
#define HOUR_PIN 17
#define NUM_PIXELS 28 // Total led per block (hour and minute)
#define SECOND_NUM_PIXELS 1 // Led per dot
#define NUM_LEDS 2 // Led per segment

class LedManager {
   public:
    static LedManager &getInstance();

    void begin();
    void end();
    void showHours(int hour);
    void showMinutes(int minutes);
    void showColon();
    void updateColorsFromPreferences();

   private:
    LedManager();
    LedManager(const LedManager &) = delete;
    LedManager &operator=(const LedManager &) = delete;

    PreferencesManager &preferencesManager;

    Adafruit_NeoPixel hoursStrip;
    Adafruit_NeoPixel minutesStrip;
    Adafruit_NeoPixel secondsStrip;
    uint32_t hourColor1, hourColor2, minuteColor1, minuteColor2, dotsColor1, dotsColor2;

    void showHourDigit(int number, int startPixel, uint32_t color);
    void showMinuteDigit(int number, int startPixel, uint32_t color);

    bool hourSegments[10][7] = {
        {true, true, true, false, true, true, true},
        {true, false, false, false, true, false, false},
        {true, true, false, true, false, true, true},
        {true, true, false, true, true, true, false},
        {true, false, true, true, true, false, false},
        {false, true, true, true, true, true, false},
        {false, true, true, true, true, true, true},
        {true, true, false, false, true, false, false},
        {true, true, true, true, true, true, true},
        {true, true, true, true, true, true, false},
    };

    bool minuteSegments[10][7] = {
        {true, true, true, false, true, true, true},
        {false, false, true, false, false, false, true},
        {true, true, false, true, false, true, true},
        {false, true, true, true, false, true, true},
        {false, false, true, true, true, false, true},
        {false, true, true, true, true, true, false},
        {true, true, true, true, true, true, false},
        {false, false, true, false, false, true, true},
        {true, true, true, true, true, true, true},
        {false, true, true, true, true, true, true},
    };
};

#endif

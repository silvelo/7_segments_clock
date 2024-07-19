#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <NTPClient.h>
#include <WiFiUdp.h>

#include "PreferencesManager.h"

class TimeManager {
   public:
    static TimeManager &getInstance();
    void begin();
    void update();
    int getHours();
    int getMinutes();
    int getSeconds();
    String getFormattedTime();
    void setTimeOffset(int offset);

   private:
    TimeManager();
    TimeManager(const TimeManager &) = delete;
    TimeManager &operator=(const TimeManager &) = delete;

    PreferencesManager &preferencesManager;

    WiFiUDP ntpUDP;
    NTPClient *timeClient;
};

#endif

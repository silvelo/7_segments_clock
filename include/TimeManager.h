#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include "PreferencesManager.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

class TimeManager
{
public:
    static TimeManager &getInstance();
    void begin();
    void update();
    int getHours();
    int getMinutes();
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

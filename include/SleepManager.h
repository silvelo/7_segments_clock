#ifndef SLEEP_MANAGER_H
#define SLEEP_MANAGER_H

#include <utility>

#include "PreferencesManager.h"

class SleepManager {
   public:
    static SleepManager &getInstance();

    void begin();
    void update(String actualDate);
    void setHours();

   private:
    SleepManager();
    SleepManager(const SleepManager &) = delete;
    SleepManager &operator=(const SleepManager &) = delete;

    PreferencesManager &preferencesManager;

    std::pair<bool, int> isCurrentTimeBetween(const String &startTime, const String &endTime,
                                              const String &currentTime);

    String startHour;
    String endHour;
};

#endif

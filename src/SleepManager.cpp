#include "SleepManager.h"

SleepManager& SleepManager::getInstance() {
    static SleepManager instance;
    return instance;
}

SleepManager::SleepManager() : preferencesManager(PreferencesManager::getInstance()) {
}

void SleepManager::begin() {
    Serial.begin(115200);

    Serial.println("Sleep Manager init...");

    setHours();
}

void SleepManager::setHours() {
    startHour = preferencesManager.getStarHour();
    endHour = preferencesManager.getEndHour();
}

bool SleepManager::update(String actualDate) {
    std::pair<bool, int> result = isCurrentTimeBetween(actualDate, startHour, endHour);
    return result.first;
}

std::pair<bool, int> SleepManager::isCurrentTimeBetween(const String& startTime,
                                                        const String& endTime,
                                                        const String& currentTime) {
    int startMinutes = (startTime.substring(0, 2).toInt() * 60) + startTime.substring(3, 5).toInt();
    int endMinutes = (endTime.substring(0, 2).toInt() * 60) + endTime.substring(3, 5).toInt();
    int currentMinutes =
        (currentTime.substring(0, 2).toInt() * 60) + currentTime.substring(3, 5).toInt();

    bool isBetween;
    int minutesToEnd;

    if (startMinutes <= endMinutes) {
        isBetween = (currentMinutes >= startMinutes) && (currentMinutes <= endMinutes);
        minutesToEnd = endMinutes - currentMinutes;
    } else {
        isBetween = (currentMinutes >= startMinutes) || (currentMinutes <= endMinutes);
        if (currentMinutes >= startMinutes) {
            minutesToEnd = (1440 - currentMinutes) + endMinutes;
        } else {
            minutesToEnd = endMinutes - currentMinutes;
        }
    }
    return std::make_pair(isBetween, minutesToEnd * 60 * 1000000);
}
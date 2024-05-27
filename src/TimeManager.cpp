#include "TimeManager.h"

TimeManager &TimeManager::getInstance() {
    static TimeManager instance;
    return instance;
}

TimeManager::TimeManager() : preferencesManager(PreferencesManager::getInstance()) {
}

void TimeManager::begin() {
    Serial.begin(115200);

    if (timeClient == nullptr) {
        timeClient = new NTPClient(ntpUDP, "pool.ntp.org");
        Serial.println("Cliente NTP inicializado.");
    }

    timeClient->begin();

    setTimeOffset(preferencesManager.getTimeOffset());

    Serial.println("Time Manager inicializado.");
}

void TimeManager::update() {
    timeClient->update();
}

int TimeManager::getHours() {
    Serial.println("Get Hours");
    return timeClient->getHours();
}

int TimeManager::getMinutes() {
    Serial.println("Get Minutes");
    return timeClient->getMinutes();
}

void TimeManager::setTimeOffset(int offset) {
    Serial.println("Change Time Offset");
    timeClient->setTimeOffset(offset * 3600);
}

String TimeManager::getFormattedTime() {
    return timeClient->getFormattedTime();
}
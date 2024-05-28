#include <ArduinoOTA.h>
#include <ElegantOTA.h>
#include <NTPClient.h>
#include <SPIFFS.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>

#include "LedManager.h"
#include "PreferencesManager.h"
#include "ServerManager.h"
#include "TimeManager.h"

ServerManager &serverManager = ServerManager::getInstance();
LedManager &ledManager = LedManager::getInstance();
PreferencesManager &preferencesManager = PreferencesManager::getInstance();
TimeManager &timeManager = TimeManager::getInstance();

bool showSeconds = true;

const char *apSSID = "NeoPixelClock";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup() {
    Serial.begin(115200);
    Serial.println();

    WiFiManager wifiManager;

    if (!wifiManager.autoConnect(apSSID)) {
        Serial.println("Failed to connect. Restarting...");
        delay(3000);
        ESP.restart();
    }

    Serial.println("Conexión exitosa!");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());

    if (!SPIFFS.begin(true)) {
        Serial.println("Error mounting file system");
        return;
    }

    serverManager.begin();
    preferencesManager.begin();
    timeManager.begin();

    AsyncWebServer *server = serverManager.getServer();

    ElegantOTA.begin(server);
    ledManager.begin();
}

void loop() {
    ElegantOTA.loop();
    timeManager.update();

    int hours = timeManager.getHours();
    int minutes = timeManager.getMinutes();

    Serial.println(timeManager.getFormattedTime());

    ledManager.showHours(hours);
    ledManager.showMinutes(minutes);
    ledManager.showColon();

    delay(1000);
}
#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "PreferencesManager.h"
#include "LedManager.h"

#define MIME_APPLICATION_JSON "application/json"

class ServerManager
{
public:
    static ServerManager &getInstance();

    AsyncWebServer *getServer();
    void begin();
    void getColors(AsyncWebServerRequest *request);
    void updateColors(AsyncWebServerRequest *request);
    void getTimezone(AsyncWebServerRequest *request);
    void updateTimezone(AsyncWebServerRequest *request);
    void getDeepSleep(AsyncWebServerRequest *request);
    void updateDeepSleep(AsyncWebServerRequest *request);

private:
    ServerManager();
    ServerManager(const ServerManager &) = delete;
    ServerManager &operator=(const ServerManager &) = delete;

    AsyncWebServer server;
    PreferencesManager &preferencesManager;
    LedManager &ledManager;
    void handleRoot(AsyncWebServerRequest *request);
    void handleCSS(AsyncWebServerRequest *request);
    void handleJS(AsyncWebServerRequest *request);
    void serveFile(const String &filePath, const String &contentType, AsyncWebServerRequest *request);
    JsonDocument createColorJson();
    uint32_t stringToHex(const String &hexString);
    String hexToString(uint32_t hex);
};

#endif

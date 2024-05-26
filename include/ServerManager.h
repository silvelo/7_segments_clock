#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "PreferencesManager.h"
#include "LedManager.h"
#include "TimeManager.h"

#define MIME_APPLICATION_JSON "application/json"

class ServerManager
{
public:
    static ServerManager &getInstance();

    AsyncWebServer *getServer();
    void begin();

private:
    ServerManager();
    ServerManager(const ServerManager &) = delete;
    ServerManager &operator=(const ServerManager &) = delete;

    AsyncWebServer server;
    PreferencesManager &preferencesManager;
    LedManager &ledManager;
    TimeManager &timeManager;

    void getColors(AsyncWebServerRequest *request);
    void updateColors(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
    void getTimezone(AsyncWebServerRequest *request);
    void updateTimezone(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
    void getDeepSleep(AsyncWebServerRequest *request);
    void updateDeepSleep(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
    void handleRoot(AsyncWebServerRequest *request);
    void handleCSS(AsyncWebServerRequest *request);
    void handleJS(AsyncWebServerRequest *request);
    void serveFile(const String &filePath, const String &contentType, AsyncWebServerRequest *request);
    JsonDocument createColorJson();
    uint32_t stringToHex(const String &hexString);
    String hexToString(uint32_t hex);
    String getBodyContent(uint8_t *data, size_t len);
};

#endif

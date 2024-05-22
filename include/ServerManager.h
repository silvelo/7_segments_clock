#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <WebServer.h>
#include <ArduinoJson.h>
#include "PreferencesManager.h"
#include "LedManager.h"

#define MIME_APPLICATION_JSON "application/json"

class ServerManager
{
public:
    static ServerManager &getInstance();

    WebServer *getServer();
    void handleClient();
    void begin();
    void handleRoot();
    void getColors();
    void updateColors();
    void getTimezone();
    void updateTimezone();
    void getDeepSleep();
    void updateDeepSleep();

private:
    ServerManager();
    ServerManager(const ServerManager &) = delete;
    ServerManager &operator=(const ServerManager &) = delete;

    WebServer server;
    PreferencesManager &preferencesManager;
    LedManager &ledManager;

    void serveFile(const String &filePath, const String &contentType);
    JsonDocument createColorJson();
    uint32_t stringToHex(const String &hexString);
    String hexToString(uint32_t hex);
};

#endif

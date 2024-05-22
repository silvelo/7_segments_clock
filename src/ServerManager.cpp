#include "ServerManager.h"
#include <SPIFFS.h>

ServerManager &ServerManager::getInstance()
{
    static ServerManager instance;
    return instance;
}

ServerManager::ServerManager()
    : server(80), preferencesManager(PreferencesManager::getInstance()), ledManager(LedManager::getInstance())
{
}

void ServerManager::begin()
{
    server.begin();
    server.on("/", HTTP_GET, std::bind(&ServerManager::handleRoot, this));
    server.on("/colors", HTTP_GET, std::bind(&ServerManager::getColors, this));
    server.on("/colors", HTTP_POST, std::bind(&ServerManager::updateColors, this));
    server.on("/deep-sleep", HTTP_GET, std::bind(&ServerManager::getDeepSleep, this));
    server.on("/deep-sleep", HTTP_POST, std::bind(&ServerManager::updateDeepSleep, this));
}

WebServer *ServerManager::getServer()
{
    return &server;
}

void ServerManager::handleClient()
{
    server.handleClient();
}

void ServerManager::handleRoot()
{
    serveFile("/index.html", "text/html");
}

void ServerManager::serveFile(const String &filePath, const String &contentType)
{
    File file = SPIFFS.open(filePath, "r");
    if (!file)
    {
        Serial.println("Error al abrir el archivo");
        server.send(500, "text/plain", "Error interno del servidor");
        return;
    }
    server.streamFile(file, contentType);
    file.close();
}

void ServerManager::getColors()
{
    JsonDocument colorData = createColorJson();

    String jsonString;
    serializeJson(colorData, jsonString);

    server.send(200, MIME_APPLICATION_JSON, jsonString);
}

uint32_t ServerManager::stringToHex(const String &hexColorString)
{
    String hexWithoutPrefix = hexColorString.substring(1);
    uint32_t hexColor = strtoul(hexWithoutPrefix.c_str(), NULL, 16);
    return hexColor;
}

String ServerManager::hexToString(uint32_t hexColor)
{
    String hexColorString = String(hexColor, HEX);
    return "#" + hexColorString;
}

void ServerManager::updateColors()
{
    if (server.args() > 0)
    {
        preferencesManager.setHourLed1Color(stringToHex(server.arg("hour_led1_color")));
        preferencesManager.setHourLed2Color(stringToHex(server.arg("hour_led2_color")));
        preferencesManager.setDotsLed1Color(stringToHex(server.arg("dots_led1_color")));
        preferencesManager.setDotsLed2Color(stringToHex(server.arg("dots_led2_color")));
        preferencesManager.setMinutesLed1Color(stringToHex(server.arg("minutes_led1_color")));
        preferencesManager.setMinutesLed2Color(stringToHex(server.arg("minutes_led2_color")));

        ledManager.updateColorsFromPreferences();
    }
    server.send(204);
}

void ServerManager::getTimezone()
{
    JsonDocument timezoneData;

    timezoneData["timeOffset"] = preferencesManager.getTimeOffset();

    String jsonString;
    serializeJson(timezoneData, jsonString);

    server.send(200, MIME_APPLICATION_JSON, jsonString);
}

void ServerManager::updateTimezone()
{
    if (server.args() > 0)
    {
        int timeOffset = server.arg("timeOffset").toInt();
        preferencesManager.setTimeOffset(timeOffset * 3600);
        // timeClient.setTimeOffset(timeOffset * 3600);
    }

    server.send(204);
}

void ServerManager::getDeepSleep()
{
    JsonDocument deepSleepData;

    deepSleepData["start_timestamp"] = preferencesManager.getStartTimestamp();
    deepSleepData["end_timestamp"] = preferencesManager.getEndTimestamp();

    String jsonString;
    serializeJson(deepSleepData, jsonString);

    server.send(200, MIME_APPLICATION_JSON, jsonString);
}

void ServerManager::updateDeepSleep()
{
    if (server.args() > 0)
    {
        preferencesManager.setStartTimestamp(server.arg("start_timestamp").toInt());
        preferencesManager.setEndTimestamp(server.arg("end_timestamp").toInt());
    }

    server.send(204);
}

JsonDocument ServerManager::createColorJson()
{
    JsonDocument colorData;

    colorData["hour_led1_color"] = hexToString(preferencesManager.getHourLed1Color());
    colorData["hour_led2_color"] = hexToString(preferencesManager.getHourLed2Color());
    colorData["dots_led1_color"] = hexToString(preferencesManager.getDotsLed1Color());
    colorData["dots_led2_color"] = hexToString(preferencesManager.getDotsLed2Color());
    colorData["minutes_led1_color"] = hexToString(preferencesManager.getMinutesLed1Color());
    colorData["minutes_led2_color"] = hexToString(preferencesManager.getMinutesLed2Color());

    return colorData;
}

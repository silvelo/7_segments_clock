#include <SPIFFS.h>
#include "ServerManager.h"

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
    if (!SPIFFS.begin())
    {
        Serial.println("Error al montar el sistema de archivos");
        return;
    }

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    server.on("/colors", HTTP_GET, std::bind(&ServerManager::getColors, this, std::placeholders::_1));
    server.on("/timezone", HTTP_GET, std::bind(&ServerManager::getTimezone, this, std::placeholders::_1));
    server.on("/deep-sleep", HTTP_GET, std::bind(&ServerManager::getDeepSleep, this, std::placeholders::_1));

    server.on("/colors", HTTP_POST, std::bind(&ServerManager::updateColors, this, std::placeholders::_1));
    server.on("/timezone", HTTP_POST, std::bind(&ServerManager::updateTimezone, this, std::placeholders::_1));
    server.on("/deep-sleep", HTTP_POST, std::bind(&ServerManager::updateDeepSleep, this, std::placeholders::_1));

    server.begin();
}

AsyncWebServer *ServerManager::getServer()
{
    return &server;
}

void ServerManager::handleRoot(AsyncWebServerRequest *request)
{
    serveFile("/index.html", "text/html", request);
}

void ServerManager::handleCSS(AsyncWebServerRequest *request)
{
    serveFile("/style.css", "text/css", request);
}

void ServerManager::handleJS(AsyncWebServerRequest *request)
{
    serveFile("/main.js", "text/javascript", request);
}

void ServerManager::getColors(AsyncWebServerRequest *request)
{
    JsonDocument colorData = createColorJson();

    String jsonString;
    serializeJson(colorData, jsonString);

    request->send(200, MIME_APPLICATION_JSON, jsonString);
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

void ServerManager::updateColors(AsyncWebServerRequest *request)
{
    if (request->hasParam("hour_led1_color") && request->hasParam("hour_led2_color") && request->hasParam("dots_led1_color") &&
        request->hasParam("dots_led2_color") && request->hasParam("minutes_led1_color") && request->hasParam("minutes_led2_color"))
    {
        preferencesManager.setHourLed1Color(stringToHex(request->getParam("hour_led1_color")->value()));
        preferencesManager.setHourLed2Color(stringToHex(request->getParam("hour_led2_color")->value()));
        preferencesManager.setDotsLed1Color(stringToHex(request->getParam("dots_led1_color")->value()));
        preferencesManager.setDotsLed2Color(stringToHex(request->getParam("dots_led2_color")->value()));
        preferencesManager.setMinutesLed1Color(stringToHex(request->getParam("minutes_led1_color")->value()));
        preferencesManager.setMinutesLed2Color(stringToHex(request->getParam("minutes_led2_color")->value()));

        ledManager.updateColorsFromPreferences();
    }
    request->send(204);
}

void ServerManager::getTimezone(AsyncWebServerRequest *request)
{
    JsonDocument timezoneData;

    timezoneData["timeOffset"] = preferencesManager.getTimeOffset();

    String jsonString;
    serializeJson(timezoneData, jsonString);

    request->send(200, MIME_APPLICATION_JSON, jsonString);
}

void ServerManager::updateTimezone(AsyncWebServerRequest *request)
{
    if (request->hasParam("timeOffset"))
    {
        int timeOffset = request->getParam("timeOffset")->value().toInt();
        preferencesManager.setTimeOffset(timeOffset * 3600);
    }

    request->send(204);
}

void ServerManager::getDeepSleep(AsyncWebServerRequest *request)
{
    JsonDocument deepSleepData;

    deepSleepData["start_timestamp"] = preferencesManager.getStartTimestamp();
    deepSleepData["end_timestamp"] = preferencesManager.getEndTimestamp();

    String jsonString;
    serializeJson(deepSleepData, jsonString);

    request->send(200, MIME_APPLICATION_JSON, jsonString);
}

void ServerManager::updateDeepSleep(AsyncWebServerRequest *request)
{
    if (request->hasParam("start_timestamp") && request->hasParam("end_timestamp"))
    {
        preferencesManager.setStartTimestamp(request->getParam("start_timestamp")->value().toInt());
        preferencesManager.setEndTimestamp(request->getParam("end_timestamp")->value().toInt());
    }

    request->send(204);
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

#include "ServerManager.h"

ServerManager &ServerManager::getInstance() {
    static ServerManager instance;
    return instance;
}

ServerManager::ServerManager()
    : server(80),
      preferencesManager(PreferencesManager::getInstance()),
      ledManager(LedManager::getInstance()),
      sleepManager(SleepManager::getInstance()),
      timeManager(TimeManager::getInstance()) {
}

void ServerManager::begin() {
    Serial.begin(115200);

    if (!SPIFFS.begin()) {
        Serial.println("Error mounting file system");
        return;
    }

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    server.on("/colors", HTTP_GET,
              std::bind(&ServerManager::getColors, this, std::placeholders::_1));
    server.on("/leds", HTTP_GET,
              std::bind(&ServerManager::getLedsPerSegment, this, std::placeholders::_1));
    server.on("/timezone", HTTP_GET,
              std::bind(&ServerManager::getTimezone, this, std::placeholders::_1));
    server.on("/deep-sleep", HTTP_GET,
              std::bind(&ServerManager::getDeepSleep, this, std::placeholders::_1));

    server.on(
        "/colors", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
        std::bind(&ServerManager::updateColors, this, std::placeholders::_1, std::placeholders::_2,
                  std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
    server.on(
        "/timezone", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
        std::bind(&ServerManager::updateTimezone, this, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,
                  std::placeholders::_5));
    server.on(
        "/leds", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
        std::bind(&ServerManager::updateLedsPerSegment, this, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,
                  std::placeholders::_5));
    server.on(
        "/deep-sleep", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
        std::bind(&ServerManager::updateDeepSleep, this, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,
                  std::placeholders::_5));

    server.begin();
    Serial.println("Servidor inicializado.");
}

AsyncWebServer *ServerManager::getServer() {
    return &server;
}

void ServerManager::handleRoot(AsyncWebServerRequest *request) {
    serveFile("/index.html", "text/html", request);
}

void ServerManager::handleCSS(AsyncWebServerRequest *request) {
    serveFile("/style.css", "text/css", request);
}

void ServerManager::handleJS(AsyncWebServerRequest *request) {
    serveFile("/main.js", "text/javascript", request);
}

void ServerManager::getColors(AsyncWebServerRequest *request) {
    Serial.println(String("[") + request->methodToString() + "] " + request->url());
    JsonDocument colorData = createColorJson();

    String jsonString;
    serializeJson(colorData, jsonString);
    request->send(200, MIME_APPLICATION_JSON, jsonString);
}

void ServerManager::updateColors(AsyncWebServerRequest *request, uint8_t *data, size_t len,
                                 size_t index, size_t total) {
    Serial.println(String("[") + request->methodToString() + "] " + request->url());
    String bodyContent = getBodyContent(data, len);

    JsonDocument body;
    DeserializationError error = deserializeJson(body, bodyContent);
    if (error) {
        request->send(400);
        return;
    }

    if (body.containsKey("hour_led1_color") && body.containsKey("hour_led2_color") &&
        body.containsKey("dots_led1_color") && body.containsKey("dots_led2_color") &&
        body.containsKey("minutes_led1_color") && body.containsKey("minutes_led2_color")) {
        preferencesManager.setHourLed1Color(stringToHex(body["hour_led1_color"]));
        preferencesManager.setHourLed2Color(stringToHex(body["hour_led2_color"]));
        preferencesManager.setDotsLed1Color(stringToHex(body["dots_led1_color"]));
        preferencesManager.setDotsLed2Color(stringToHex(body["dots_led2_color"]));
        preferencesManager.setMinutesLed1Color(stringToHex(body["minutes_led1_color"]));
        preferencesManager.setMinutesLed2Color(stringToHex(body["minutes_led2_color"]));

        ledManager.updateColorsFromPreferences();

        request->send(204);
        return;
    }

    request->send(400);
}

void ServerManager::getLedsPerSegment(AsyncWebServerRequest *request) {
    Serial.println(String("[") + request->methodToString() + "] " + request->url());
    JsonDocument ledsPerSegmentData;

    ledsPerSegmentData["ledsPerSegment"] = preferencesManager.getLedsPerSegment();

    String jsonString;
    serializeJson(ledsPerSegmentData, jsonString);

    request->send(200, MIME_APPLICATION_JSON, jsonString);
}

void ServerManager::updateLedsPerSegment(AsyncWebServerRequest *request, uint8_t *data, size_t len,
                                         size_t index, size_t total) {
    Serial.println(String("[") + request->methodToString() + "] " + request->url());
    String bodyContent = getBodyContent(data, len);

    JsonDocument body;
    DeserializationError error = deserializeJson(body, bodyContent);
    if (error) {
        request->send(400);
        return;
    }

    if (body.containsKey("ledsPerSegment")) {
        int ledsPerSegment = body["ledsPerSegment"].as<int>();
        preferencesManager.setLedsPerSegment(ledsPerSegment);
        ledManager.updateSegmentsFromPreferences();
        request->send(204);
        return;
    }

    request->send(400);
}

void ServerManager::getTimezone(AsyncWebServerRequest *request) {
    Serial.println(String("[") + request->methodToString() + "] " + request->url());
    JsonDocument timezoneData;

    timezoneData["timeOffset"] = preferencesManager.getTimeOffset();

    String jsonString;
    serializeJson(timezoneData, jsonString);

    request->send(200, MIME_APPLICATION_JSON, jsonString);
}

void ServerManager::updateTimezone(AsyncWebServerRequest *request, uint8_t *data, size_t len,
                                   size_t index, size_t total) {
    Serial.println(String("[") + request->methodToString() + "] " + request->url());
    String bodyContent = getBodyContent(data, len);

    JsonDocument body;
    DeserializationError error = deserializeJson(body, bodyContent);
    if (error) {
        request->send(400);
        return;
    }

    if (body.containsKey("timeOffset")) {
        int timeOffset = body["timeOffset"].as<int>();
        preferencesManager.setTimeOffset(timeOffset);
        timeManager.setTimeOffset(timeOffset);

        request->send(204);
        return;
    }

    request->send(400);
}

void ServerManager::getDeepSleep(AsyncWebServerRequest *request) {
    Serial.println(String("[") + request->methodToString() + "] " + request->url());
    JsonDocument deepSleepData;

    deepSleepData["start_timestamp"] = preferencesManager.getStarHour();
    deepSleepData["end_timestamp"] = preferencesManager.getEndHour();

    String jsonString;
    serializeJson(deepSleepData, jsonString);

    request->send(200, MIME_APPLICATION_JSON, jsonString);
}

void ServerManager::updateDeepSleep(AsyncWebServerRequest *request, uint8_t *data, size_t len,
                                    size_t index, size_t total) {
    Serial.println(String("[") + request->methodToString() + "] " + request->url());
    String bodyContent = getBodyContent(data, len);

    JsonDocument body;
    DeserializationError error = deserializeJson(body, bodyContent);
    if (error) {
        request->send(400);
        return;
    }

    if (body.containsKey("start_timestamp") && body.containsKey("end_timestamp")) {
        preferencesManager.setStartHour(body["start_timestamp"]);
        preferencesManager.setEndHour(body["end_timestamp"]);
        sleepManager.setHours();
        request->send(204);
        return;
    }

    request->send(400);
}

JsonDocument ServerManager::createColorJson() {
    JsonDocument colorData;

    colorData["hour_led1_color"] = hexToString(preferencesManager.getHourLed1Color());
    colorData["hour_led2_color"] = hexToString(preferencesManager.getHourLed2Color());
    colorData["dots_led1_color"] = hexToString(preferencesManager.getDotsLed1Color());
    colorData["dots_led2_color"] = hexToString(preferencesManager.getDotsLed2Color());
    colorData["minutes_led1_color"] = hexToString(preferencesManager.getMinutesLed1Color());
    colorData["minutes_led2_color"] = hexToString(preferencesManager.getMinutesLed2Color());

    return colorData;
}

uint32_t ServerManager::stringToHex(const String &hexColorString) {
    String hexWithoutPrefix = hexColorString.substring(1);
    uint32_t hexColor = strtoul(hexWithoutPrefix.c_str(), NULL, 16);
    return hexColor;
}

String ServerManager::hexToString(uint32_t hexColor) {
    String hexColorString = String(hexColor, HEX);
    return "#" + hexColorString;
}

String ServerManager::getBodyContent(uint8_t *data, size_t len) {
    String content = "";
    for (size_t i = 0; i < len; i++) {
        content.concat((char)data[i]);
    }
    return content;
}
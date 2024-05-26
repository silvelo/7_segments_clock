#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <SPIFFS.h>
#include <WiFiManager.h>
#include "ServerManager.h"
#include <ElegantOTA.h>
#include "LedManager.h"

ServerManager &serverManager = ServerManager::getInstance();
LedManager &ledManager = LedManager::getInstance();

bool showSeconds = true;

const char *apSSID = "NeoPixelClock";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup()
{
  Serial.begin(115200);
  Serial.println();

  WiFiManager wifiManager;

  if (!wifiManager.autoConnect(apSSID))
  {
    Serial.println("Fallo al conectar. Reiniciando...");
    delay(3000);
    ESP.restart();
  }

  Serial.println("Conexión exitosa!");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  if (!SPIFFS.begin(true))
  {
    Serial.println("Error al inicializar SPIFFS");
    return;
  }

  serverManager.begin();

  timeClient.begin();
  AsyncWebServer *server = serverManager.getServer();

  ElegantOTA.begin(server);
  ledManager.initLEDs();
}

void loop()
{
  ElegantOTA.loop();
  timeClient.update();

  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();

  Serial.println(timeClient.getFormattedTime());

  ledManager.showHours(hours);
  ledManager.showMinutes(minutes);
  ledManager.showColon();

  delay(1000);
}
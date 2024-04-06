#include <Adafruit_NeoPixel.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <ElegantOTA.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <Preferences.h>

#define MINUTE_PIN 16
#define SECOND_PIN 18
#define HOUR_PIN 17
#define NUMPIXELS 28
#define SECOND_NUMPIXELS 2

Adafruit_NeoPixel hoursStrip = Adafruit_NeoPixel(NUMPIXELS, HOUR_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel minutesStrip = Adafruit_NeoPixel(NUMPIXELS, MINUTE_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel secondsStrip = Adafruit_NeoPixel(SECOND_NUMPIXELS, SECOND_PIN, NEO_GRB + NEO_KHZ800);

bool showSeconds = true;

uint32_t hour_led1_color;
uint32_t hour_led2_color;
uint32_t dots_led1_color;
uint32_t dots_led2_color;
uint32_t minutes_led1_color;
uint32_t minutes_led2_color;

const char *apSSID = "NeoPixelClock";
WebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

uint32_t stringToHex(String hexColorString)
{
  hexColorString.remove(0, 1);
  uint32_t hexColor = strtoul(hexColorString.c_str(), NULL, 16);
  return hexColor;
}

String hexToString(uint32_t hexColor)
{
  String hexColorString = String(hexColor, HEX);
  return "#" + hexColorString;
}

void initStrip(Adafruit_NeoPixel *strip)
{
  strip->begin();
  strip->show();
}

void handleRoot()
{
  File file = SPIFFS.open("/index.html", "r");
  if (!file)
  {
    Serial.println("Error al abrir el archivo");
    server.send(500, "text/plain", "Error interno del servidor");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

void handleColors()
{
  JsonDocument colorData;

  colorData["hour_led1_color"] = hexToString(hour_led1_color);
  colorData["hour_led2_color"] = hexToString(hour_led2_color);
  colorData["dots_led1_color"] = hexToString(dots_led1_color);
  colorData["dots_led2_color"] = hexToString(dots_led2_color);
  colorData["minutes_led1_color"] = hexToString(minutes_led1_color);
  colorData["minutes_led2_color"] = hexToString(minutes_led2_color);

  String jsonString;
  serializeJson(colorData, jsonString);

  server.send(200, "application/json", jsonString);
}

void saveTimeOffset(int timeOffset)
{
  Preferences preferences;
  preferences.begin(apSSID, false);
  preferences.putInt("timeOffset", timeOffset);
  Serial.print("Save TimeOffset: ");
  Serial.println(timeOffset);
  preferences.end();
}

void saveLedColor()
{
  Preferences preferences;
  preferences.begin(apSSID, false);

  preferences.putUInt("hour_led1_color", hour_led1_color);
  preferences.putUInt("hour_led2_color", hour_led2_color);
  preferences.putUInt("dots_led1_color", dots_led1_color);
  preferences.putUInt("dots_led2_color", dots_led2_color);
  preferences.putUInt("m_led1_color", minutes_led1_color);
  preferences.putUInt("m_led2_color", minutes_led2_color);

  Serial.println("Save Colors: ");
  Serial.print("hour_led1_color");
  Serial.println(hour_led1_color);
  Serial.print("hour_led2_color");
  Serial.println(hour_led2_color);
  Serial.print("dots_led1_color");
  Serial.println(dots_led1_color);
  Serial.print("dots_led2_color");
  Serial.println(dots_led2_color);
  Serial.print("minutes_led1_color");
  Serial.println(minutes_led1_color);
  Serial.print("minutes_led2_color");
  Serial.println(minutes_led2_color);

  preferences.end();
}

void setTimeZone()
{
  Preferences preferences;
  preferences.begin(apSSID, false);
  int timeOffset = preferences.getInt("timeOffset", 0);
  preferences.end();
  Serial.print("Configure Save TimeOffset: ");
  Serial.println(timeOffset);
  
  timeClient.setTimeOffset(timeOffset*3600);

}

void configureLedColors()
{
  uint32_t redRGBColor = stringToHex("#FF0000");

  Preferences preferences;
  preferences.begin(apSSID, false);

  hour_led1_color = preferences.getUInt("hour_led1_color", redRGBColor);
  hour_led2_color = preferences.getUInt("hour_led2_color", redRGBColor);
  dots_led1_color = preferences.getUInt("dots_led1_color", redRGBColor);
  dots_led2_color = preferences.getUInt("dots_led2_color", redRGBColor);
  minutes_led1_color = preferences.getUInt("m_led1_color", redRGBColor);
  minutes_led2_color = preferences.getUInt("m_led2_color", redRGBColor);

  Serial.println("Configure Save Colors: ");
  Serial.print("hour_led1_color");
  Serial.println(hour_led1_color);
  Serial.print("hour_led2_color");
  Serial.println(hour_led2_color);
  Serial.print("dots_led1_color");
  Serial.println(dots_led1_color);
  Serial.print("dots_led2_color");
  Serial.println(dots_led2_color);
  Serial.print("minutes_led1_color");
  Serial.println(minutes_led1_color);
  Serial.print("minutes_led2_color");
  Serial.println(minutes_led2_color);

  preferences.end();
}

void updateTimeZone()
{
  if (server.args() > 0)
  {
    int timeOffset = server.arg("timeOffset").toInt();
    timeClient.setTimeOffset(timeOffset * 3600);
    saveTimeOffset(timeOffset);
  }

  server.send(204);
}

void handleChangeColor()
{
  if (server.args() > 0)
  {

    hour_led1_color = stringToHex(server.arg("hour_led1_color"));
    hour_led2_color = stringToHex(server.arg("hour_led2_color"));
    dots_led1_color = stringToHex(server.arg("dots_led1_color"));
    dots_led2_color = stringToHex(server.arg("dots_led2_color"));
    minutes_led1_color = stringToHex(server.arg("minutes_led1_color"));
    minutes_led2_color = stringToHex(server.arg("minutes_led2_color"));

    saveLedColor();
  }
  server.send(204);
}

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

  server.on("/", HTTP_GET, handleRoot);
  server.on("/timezone", HTTP_POST, updateTimeZone);
  server.on("/colors", HTTP_GET, handleColors);
  server.on("/colors", HTTP_POST, handleChangeColor);

  server.begin();
  EEPROM.begin(512);

  configureLedColors();
  timeClient.begin();
  setTimeZone();
  
  ElegantOTA.begin(&server);

  initStrip(&minutesStrip);
  initStrip(&hoursStrip);
}

void showHours(Adafruit_NeoPixel *strip, int number, int startPixel)
{
  bool segments[10][7] = {
      {true, true, true, false, true, true, true},
      {true, false, false, false, true, false, false},
      {true, true, false, true, false, true, true},
      {true, true, false, true, true, true, false},
      {true, false, true, true, true, false, false},
      {false, true, true, true, true, true, false},
      {false, true, true, true, true, true, true},
      {true, true, false, false, true, false, false},
      {true, true, true, true, true, true, true},
      {true, true, true, true, true, true, false},
  };

  for (int i = 0; i < 7; i++)
  {
    if (segments[number][i])
    {
      strip->setPixelColor(startPixel + (i * 2), hour_led1_color);
      strip->setPixelColor(startPixel + (i * 2) + 1, hour_led2_color);
    }
    else
    {
      strip->setPixelColor(startPixel + (i * 2), strip->Color(0, 0, 0));
      strip->setPixelColor(startPixel + (i * 2) + 1, strip->Color(0, 0, 0));
    }
  }
  strip->show();
}

void showMinutes(Adafruit_NeoPixel *strip, int number, int startPixel)
{
  bool segments[10][7] = {
      {true, true, true, false, true, true, true},
      {false, false, true, false, false, false, true},
      {true, true, false, true, false, true, true},
      {false, true, true, true, false, true, true},
      {false, false, true, true, true, false, true},
      {false, true, true, true, true, true, false},
      {true, true, true, true, true, true, false},
      {false, false, true, false, false, true, true},
      {true, true, true, true, true, true, true},
      {false, true, true, true, true, true, true},
  };

  for (int i = 0; i < 7; i++)
  {
    if (segments[number][i])
    {
      strip->setPixelColor(startPixel + (i * 2), minutes_led1_color);
      strip->setPixelColor(startPixel + (i * 2) + 1, minutes_led2_color);
    }
    else
    {
      strip->setPixelColor(startPixel + (i * 2), strip->Color(0, 0, 0));
      strip->setPixelColor(startPixel + (i * 2) + 1, strip->Color(0, 0, 0));
    }
  }
  strip->show();
}

void showColon()
{
  if (showSeconds)
  {
    secondsStrip.setPixelColor(0, dots_led1_color);
    secondsStrip.setPixelColor(1, dots_led2_color);
  }
  else
  {
    secondsStrip.setPixelColor(0, secondsStrip.Color(0, 0, 0));
    secondsStrip.setPixelColor(1, secondsStrip.Color(0, 0, 0));
  }

  showSeconds = !showSeconds;
  secondsStrip.show();
}

void loop()
{
  server.handleClient();
  ElegantOTA.loop();
  timeClient.update();

  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();

  Serial.println(timeClient.getFormattedTime());

  showHours(&hoursStrip, hours % 10, 0);
  showHours(&hoursStrip, hours / 10, NUMPIXELS / 2);

  showMinutes(&minutesStrip, minutes / 10, 0);
  showMinutes(&minutesStrip, minutes % 10, NUMPIXELS / 2);

  showColon();

  delay(1000);
}
#include "TWeb.h"

void TWeb::begin() {
  Serial.print(F("*WEB: Server initializing "));

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/style.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/script.js", "text/JavaScript");
  });

  server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plane", timeNow.getTimeString());
  });

  server.on("/pins", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plane", TPin::getPinsStatusHtml());
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("*WEB: GET[" + String(request->args()) +
                   "]: " + request->getParam("element")->value() + ", " +
                   request->getParam("value")->value());
    if (request->hasParam("element")) {
      String webElement = request->getParam("element")->value();
      uint8_t pinValue = request->hasParam("value")
                             ? request->getParam("value")->value().toInt()
                             : 0;
      uint8_t pinNumber = TPin::getPinNumber(
          webElement.indexOf("D") >= 0 ? webElement.substring(1).toInt() : 255);
      if (pinNumber < 255) digitalWrite(pinNumber, pinValue);
    }
    request->send(200, "text/plain", "OK");
  });

  // Enabling Async Mode
  // Open ElegantOTA folder and then open src folder
  // Locate the ELEGANTOTA_USE_ASYNC_WEBSERVER macro in the ElegantOTA.h file,
  // and set it to 1: #define ELEGANTOTA_USE_ASYNC_WEBSERVER 1
  ElegantOTA.begin(&server);
  server.onNotFound(notFound);
  server.begin();
  Serial.println(F("[OK]"));
}

String TWeb::processor(const String &var) {
  String returnString = "";
  if (var == "BOARDNAME") returnString = preferences.getString("ssid");
  if (var == "D0_CHECKED") returnString = digitalRead(D0) ? "checked" : "";
  if (var == "D1_CHECKED") returnString = digitalRead(D1) ? "checked" : "";
  if (var == "D2_CHECKED") returnString = digitalRead(D2) ? "checked" : "";
  if (var == "D3_CHECKED") returnString = digitalRead(D3) ? "checked" : "";
  if (var == "D4_CHECKED") returnString = digitalRead(D4) ? "checked" : "";
  if (var == "D5_CHECKED") returnString = digitalRead(D5) ? "checked" : "";
  if (var == "D6_CHECKED") returnString = digitalRead(D6) ? "checked" : "";
  if (var == "D7_CHECKED") returnString = digitalRead(D7) ? "checked" : "";
  if (var == "D8_CHECKED") returnString = digitalRead(D8) ? "checked" : "";
  return returnString;
}

void TWeb::notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

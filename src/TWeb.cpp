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
    request->send(200, "text/plane", timeNow.getOnlineString());
  });

  server.on("/pins", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plane", pins.getPinsStatus(true));
  });

  server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plane", pins.getPinStateLog(10, true));
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("element")) {
      String webElement = request->getParam("element")->value();
      uint8_t pinValue = request->hasParam("value")
                             ? request->getParam("value")->value().toInt()
                             : 0;

      Serial.print("*WEB: GET[" + String(request->args()) +
                   "]: " + request->getParam("element")->value() + ", " +
                   request->getParam("value")->value());
      Serial.println(pins.setPin(webElement, pinValue) == 0 ? " -> OK"
                                                            : " -> ERROR");
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

  if (var == "BOARDNAME")
    returnString = preferences.getString("ssid");
  else if (var == "D0_CHECKED")
    returnString = pins.getPin("D0") ? "checked" : "";
  else if (var == "D1_CHECKED")
    returnString = pins.getPin("D1") ? "checked" : "";
  else if (var == "D2_CHECKED")
    returnString = pins.getPin("D2") ? "checked" : "";
  else if (var == "D3_CHECKED")
    returnString = pins.getPin("D3") ? "checked" : "";
  else if (var == "D4_CHECKED")
    returnString = pins.getPin("D4") ? "checked" : "";
  else if (var == "D5_CHECKED")
    returnString = pins.getPin("D5") ? "checked" : "";
  else if (var == "D6_CHECKED")
    returnString = pins.getPin("D6") ? "checked" : "";
  else if (var == "D7_CHECKED")
    returnString = pins.getPin("D7") ? "checked" : "";
  else if (var == "D8_CHECKED")
    returnString = pins.getPin("D8") ? "checked" : "";

  if (returnString == "checked")
    returnString += pins.isInput(var.substring(0, 2)) ? " disabled" : "";

  return returnString;
}

void TWeb::notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

#include "TWeb.h"

void TWeb::begin() {
  Serial.print(F("[WEB] Server initializing "));

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/style.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/script.js", "text/JavaScript");
  });

  server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plane", time_now.fetch_string_long());
  });

  server.on("/time-set", HTTP_GET, [](AsyncWebServerRequest *request) {
    String year = request->getParam("year")->value();
    String month = request->getParam("month")->value();
    String day = request->getParam("day")->value();
    String hours = request->getParam("hours")->value();
    String minutes = request->getParam("minutes")->value();
    if (year != "" && month != "" && day != "" && hours != "" &&
        minutes != "") {
      RtcDateTime time = RtcDateTime(year.toInt(), month.toInt(), day.toInt(),
                                     hours.toInt(), minutes.toInt(), 0);
      rtc.set_time(time);
      time_now.set_time(time);
      log_system.append("[WEB] Time set to: " + String(day.toInt()) + "." +
                        String(month.toInt()) + "." + String(year.toInt()) +
                        " " + String(hours.toInt()) + ":" +
                        String(minutes.toInt()) +
                        " (time: " + time_now.fetch_unixtime() +
                        ", rtc: " + rtc.getDateTime().Unix32Time() + ")");
    }
    request->send(200, "text/plain", "OK");
  });

  server.on("/online", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plane", time_now.fetch_online_string());
  });

  server.on("/pins-a", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("pin")) {
      uint16_t pin_index = request->getParam("pin")->value().toInt();
      uint16_t pin_count =
          static_cast<uint16_t>(pins.get_pins_a_state().size());

      if (pin_index >= 0 && pin_index < pin_count) {
        request->send(200, "text/plain",
                      String(pins.get_pins_a_state()[pin_index]));
      } else {
        request->send(400, "text/plain", "Invalid pin index.");
      }
    } else {
      request->send(400, "text/plain", "Pin parameter missing.");
    }
  });

  server.on("/pins-d", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("pin")) {
      uint16_t pin_index = request->getParam("pin")->value().toInt();
      uint16_t pin_count =
          static_cast<uint16_t>(pins.get_pins_d_state().size());

      if (pin_index >= 0 && pin_index < pin_count) {
        request->send(200, "text/plain",
                      String(pins.get_pins_d_state()[pin_index]));
      } else {
        request->send(400, "text/plain", "Invalid pin index.");
      }
    } else {
      request->send(400, "text/plain", "Pin parameter missing.");
    }
  });

  server.on("/log-pin-buffer", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plane", pins.fetch_pins_log_buffer(10, true));
  });

  server.on("/log-pin", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plane", sd_card.read("pin.log"));
  });

  server.on("/log-system", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plane", sd_card.read("system.log"));
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", String(), false, processor_index);
  });

  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/settings.html", String(), false,
                  processor_settings);
  });

  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("element")) {
      String webElement = request->getParam("element")->value();
      uint8_t pinValue = request->hasParam("value")
                             ? request->getParam("value")->value().toInt()
                             : 0;
      String value =
          request->getParam("value")->value() == "1" ? "ON" : "OFF";  //
      String status =
          pins.set_pin(webElement, pinValue) == 0 ? " -> OK" : " -> ERROR";
      Serial.println("[WEB] " + request->getParam("element")->value() + " = " +
                     value + status);
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

String TWeb::processor_index(const String &var) {
  String returnString = "";

  if (var == "BOARDNAME")
    returnString = preferences.getString("ssid");
  else if (var == "D0_CHECKED")
    returnString = pins.get_pin("D0") ? "checked" : "";
  else if (var == "D1_CHECKED")
    returnString = pins.get_pin("D1") ? "checked" : "";
  else if (var == "D2_CHECKED")
    returnString = pins.get_pin("D2") ? "checked" : "";
  else if (var == "D3_CHECKED")
    returnString = pins.get_pin("D3") ? "checked" : "";
  else if (var == "D4_CHECKED")
    returnString = pins.get_pin("D4") ? "checked" : "";
  else if (var == "D5_CHECKED")
    returnString = pins.get_pin("D5") ? "checked" : "";
  else if (var == "D6_CHECKED")
    returnString = pins.get_pin("D6") ? "checked" : "";
  else if (var == "D7_CHECKED")
    returnString = pins.get_pin("D7") ? "checked" : "";
  else if (var == "D8_CHECKED")
    returnString = pins.get_pin("D8") ? "checked" : "";

  if (var.substring(0, 1) == "D")
    returnString += pins.is_input(var.substring(0, 2)) ? " disabled" : "";

  return returnString;
}

String TWeb::processor_settings(const String &var) {
  String returnString = "";

  if (var == "BOARDNAME") returnString = preferences.getString("ssid");

  return returnString;
}

void TWeb::notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

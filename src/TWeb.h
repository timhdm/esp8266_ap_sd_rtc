/**
 * @file TWeb.h
 * @brief Web server functions
 * @version 0.1.1
 * @date 16-10-2024
 */

#pragma once

// https://github.com/esphome/ESPAsyncWebServer

#define WEBSERVER_H  // https://github.com/me-no-dev/ESPAsyncWebServer/issues/418#issuecomment-667976368

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <LittleFS.h>
#include <Preferences.h>
#include <RtcDS1307.h>

#include "TPin.h"
#include "TTime.h"

// main.h
extern TTime time_now;
extern TPin pins;
extern Preferences preferences;

class TWeb {
 public:
  TWeb() : server(80) {}

  void begin();

 private:
  AsyncWebServer server;

  static String processor_index(const String& var);
  static String processor_settings(const String& var);
  static void notFound(AsyncWebServerRequest* request);
};

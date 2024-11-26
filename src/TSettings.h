/**
 * @file TSettings.h
 * @brief Settings management functions
 * @version 0.1.0
 * @date 25-11-2024
 */

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <TSdCard.h>

class TSettings {
 public:
  TSettings(TSdCard *sd_card) : sd_card(sd_card), file_name("settings.json") {
    load();
  };
  void begin() { load(); };
  void save();
  String get(const String &key);
  void set(const String &key, const String &value);

 private:
  TSdCard *sd_card;
  JsonDocument json_doc;
  const char *file_name;
  void load();
};
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
  String getString(const String &key, String default_value);
  uint32_t getInt(const String &key, uint32_t default_value);
  boolean getBoolean(const String &key, boolean default_value);

  void set(const String &key, const String &value);
  void set(const String &key, const uint32_t &value);
  void set(const String &key, const boolean &value);

 private:
  TSdCard *sd_card;
  JsonDocument json_doc;
  const char *file_name;
  void load();
};
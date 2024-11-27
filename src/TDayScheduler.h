/**
 * @file TDayScheduler.h
 * @brief Class for working with day scheduler
 * @version 0.2.0
 * @date 18-11-2024
 */

#pragma once

#include <Arduino.h>
#include <Preferences.h>

#include "TLog.h"
#include "TSettings.h"

#define PIN_MAX 2         // Максимальное количество обслуживаемых пинов
#define DURATION_MAX 180  // Максимальное количество минут включения таймера

class TDayScheduler {
 public:
  TDayScheduler(TLog& log, TSettings& settings)
      : log(log),
        settings(settings),
        enabled(PIN_MAX, false),
        hour(PIN_MAX, 0),
        minute(PIN_MAX, 0),
        duration(PIN_MAX, 0) {};

  void load();
  void save();
  void clean(uint8_t pin);
  void clean_all();
  bool get_status(uint8_t pin, const time_t& now_unixtime);
  void set_scheduler(uint8_t pin, bool enabled, uint16_t hour, uint16_t minute,
                     uint16_t duration);

 private:
  TLog& log;
  TSettings& settings;
  std::vector<bool> enabled;
  std::vector<uint16_t> hour;
  std::vector<uint16_t> minute;
  std::vector<uint16_t> duration;
  bool is_duration_valid(uint16_t duration);
  String to_string();
};
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

class TDayScheduler {
 public:
  TDayScheduler(uint8_t pin, TLog& log, Preferences& preferences)
      : pin(pin),
        log(log),
        preferences(preferences),
        enabled(false),
        hour(0),
        minute(0),
        duration_in_seconds(0),
        start_unixtime(0) {};

  bool get_status(const time_t& now_unixtime);
  void set(const time_t& now_unixtime, uint16_t hour, uint16_t minute,
           uint16_t duration_in_seconds, bool enabled = false);
  void save();
  void load(const time_t& now_unixtime);
  bool is_enabled() const { return enabled; }
  void enable() { enabled = true; }
  void disable() { enabled = false; }

 private:
  uint8_t pin;
  TLog& log;
  Preferences& preferences;
  bool enabled;
  uint16_t hour;
  uint16_t minute;
  uint16_t duration_in_seconds;
  time_t start_unixtime;

  time_t get_next_unixtime() const { return start_unixtime; }
  static bool is_hour_valid(uint8_t hour);
  static bool is_minutes_valid(uint8_t hour);
  bool is_duration_valid(uint8_t duration_in_seconds);
  String to_string();
};
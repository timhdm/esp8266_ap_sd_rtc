/**
 * @file TDayScheduler.h
 * @brief Class for working with day scheduler
 * @version 0.1.0
 * @date 17-10-2024
 */

#pragma once

#include <Arduino.h>

#include "TLog.h"

extern TLog log_system;

class TDayScheduler {
 public:
  TDayScheduler()
      : id(255),
        enabled(false),
        hour(0),
        minute(0),
        start_unixtime(0),
        end_unixtime(0),
        duration_in_seconds(0) {};

  bool is_enabled() const { return enabled; }
  void enable() { enabled = true; }
  void disable() { enabled = false; }
  void set(uint8_t id, const time_t& now_unixtime, uint16_t hour,
           uint16_t minute, uint16_t duration_in_seconds, bool enabled = false);
  bool is_up(const time_t& now_unixtime);

 private:
  uint8_t id;
  bool enabled;
  uint16_t hour;
  uint16_t minute;
  bool up;
  time_t start_unixtime;
  time_t end_unixtime;
  uint16_t duration_in_seconds;

  time_t get_next_unixtime() const { return start_unixtime; }
};
/**
 * @file TDayScheduler.h
 * @brief Class for working with day scheduler
 * @version 0.1.0
 * @date 17-10-2024
 */

#pragma once

#include <Arduino.h>

class TDayScheduler {
 public:
  TDayScheduler() : enabled(false), next_unixtime(0), duration_in_seconds(0) {};

  void begin(const time_t& now_unixtime, uint16_t hour, uint16_t minute,
             uint16_t duration_in_seconds);
  bool is_enabled() const { return enabled; }
  void enable() { enabled = true; }
  void disable() { enabled = false; }
  time_t get_next_unixtime() const { return next_unixtime; }
  void set_duration(uint16_t duration_in_seconds) {
    this->duration_in_seconds = duration_in_seconds;
  }
  void set_schedule(const time_t& now_unixtime, uint16_t hour, uint16_t minute);
  bool is_time(const time_t& now_unixtime);

 private:
  bool enabled;
  time_t next_unixtime;
  uint16_t duration_in_seconds;
};
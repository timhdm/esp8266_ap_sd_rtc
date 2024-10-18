/**
 * @file TTime.h
 * @brief Time management functions
 * @version 0.1.0
 * @date 12-10-2024
 */

#pragma once

#include <Arduino.h>
#include <RtcDS1307.h>
#include <Wire.h>

#include <ctime>

class TTime {
 public:
  TTime() : now_unixtime(0), start_unixtime(0), previous_millis(millis()) {}

  void update();
  void set_time(RtcDateTime time);
  const time_t fetch_now_unixtime();
  String fetch_now_string_long();
  String fetch_now_string_short();
  String fetch_online_string();

 private:
  time_t now_unixtime;
  time_t start_unixtime;
  time_t previous_millis;
};
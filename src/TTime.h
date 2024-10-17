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
  TTime() : rtc(Wire), time_now_unix(time(nullptr)), start_time(0) {}

  void begin();
  const time_t fetch_time_now_unix();
  String fetch_time_now_string();
  String fetch_time_now_string_short();
  time_t fetch_online_seconds();
  String fetch_online_string();
  void set_time(RtcDateTime time);

 private:
  RtcDS1307<TwoWire> rtc;
  time_t time_now_unix;
  tm time_now_structure;
  time_t start_time;

  time_t update();
  bool wasError(const char* errorTopic = "");
};
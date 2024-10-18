/**
 * @file TRtc.h
 * @brief Class for working with RTC
 * @version 0.1.0
 * @date 18-10-2024
 */

#pragma once

#include <Arduino.h>
#include <RtcDS1307.h>
#include <Wire.h>

#include <ctime>

class TRtc {
 public:
  TRtc() : rtc(Wire) {}

  RtcDateTime getDateTime();
  void begin(int sda, int scl);
  void set_time(RtcDateTime time);

 private:
  RtcDS1307<TwoWire> rtc;

  bool wasError(const char* errorTopic = "");
};
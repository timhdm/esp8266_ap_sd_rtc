#pragma once

#include <Arduino.h>

#include "time.h"

class TTime {
 public:
  TTime() : unixTimeNow(0), startTime(0) {}

  time_t getTime();
  String getTimeString();
  time_t getOnlineSeconds();
  String getOnlineString();

 private:
  time_t unixTimeNow;
  tm timeStructureNow;
  time_t startTime;

  void update();
};
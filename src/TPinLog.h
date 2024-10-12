#pragma once
#include <Arduino.h>

#include <vector>

#include "TSdCard.h"
#include "TTime.h"

extern TTime time_now;

class TPinLog {
  struct PinState {
    String pin;
    uint8_t state;
    time_t timestamp;

    PinState(String pin, uint8_t state);
    String to_string();
  };

 public:
  void append(String pin, uint8_t state);
  String fetch(const size_t number = 1);

  TPinLog() : sd_log_file_name("pin.log") {};

 private:
  std::vector<PinState> pinLog;
  TSdCard sd_log_file;
  String sd_log_file_name;

  String printTimestamp(time_t timestamp);
};
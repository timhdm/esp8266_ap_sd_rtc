#pragma once
#include <Arduino.h>

#include <map>

#include "TSdCard.h"
#include "TTime.h"

extern TTime time_now;
enum PinStatus { INPUT_PIN, OUTPUT_PIN, UNDEFINED_PIN };

class TPinLog {
  struct PinState {
    String pin;
    uint8_t state;
    time_t timestamp;

    PinState(String pin, uint8_t state);
    String to_string();
  };

 public:
  void begin();
  void append(String pin, uint8_t state);
  String fetch(const size_t number = 1);

  TPinLog() : sd_log_file_name("pin.log") {};

 private:
  std::vector<PinState> pinLog;
  TSdCard sd_log_file;
  String sd_log_file_name;

  String printTimestamp(time_t timestamp);
};

class TPin {
 public:
  TPin(std::vector<PinStatus> pins_status);
  void begin();
  uint8_t convert_string_pin(String pin);
  String get_pins_status(const bool html = false);
  String get_pins_log(const int rows = 10, const bool html = false);
  uint8_t get_pin(String pin);
  uint8_t set_pin(String pin, uint8_t value);
  boolean is_output(String pin);
  boolean is_input(String pin) { return !is_output(pin); };

 private:
  std::vector<PinStatus> pins_status;

  TPinLog pinlog;

  String replace_line_breaker_to_html(String input);
  bool is_valid_pin(String pin);
};

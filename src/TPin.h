/**
 * @file TPin.h
 * @brief Class for working with pins
 * @version 0.1.1
 * @date 17-10-2024
 */

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
  TPinLog() : sd_log_file(nullptr), sd_log_file_name("pin.log") {};

  void begin(TSdCard *sd_log_file);
  void append(String pin, uint8_t state);
  String fetch(const size_t rows = 1);

 private:
  std::vector<PinState> pin_log_buffer;
  TSdCard *sd_log_file;
  const char *sd_log_file_name;

  String print_timestamp(time_t timestamp);
};

class TPin {
 public:
  TPin(std::vector<PinStatus> pins_d_mode);

  void begin(TSdCard *sd_log_file);
  uint8_t get_pin(String pin);
  uint8_t set_pin(String pin, uint8_t value);
  const std::vector<uint8_t> &get_pins_d_state();
  const std::vector<uint16_t> &get_pins_a_state();
  String fetch_pins_log_buffer(const size_t rows = 10, const bool html = false);
  String fetch_pins_log_sd(const size_t rows = 0, const bool html = false);

  boolean is_output(String pin);
  boolean is_input(String pin) { return !is_output(pin); };
  uint8_t get_pin_id_from_string(String pin);

 private:
  std::vector<PinStatus> pins_d_mode;
  std::vector<uint8_t> pins_d_state;
  std::vector<uint16_t> pins_a_state;
  TPinLog pin_log;

  void update_pins_d_state();
  void update_pins_a_state();
  String replace_lb_to_br(String input);
  bool is_valid_pin(String pin);
};

#pragma once
#include <Arduino.h>

#include <map>

#include "TPinStateLog.h"

class TPin {
 public:
  TPin();
  uint8_t convertPin(String pin);
  String getPinsStatus(const bool html = false);
  String getPinStateLog(const int number = 10, const bool html = false);
  uint8_t getPin(String pin);
  uint8_t setPin(String pin, uint8_t value);
  boolean isOutput(String pin);
  boolean isInput(String pin) { return !isOutput(pin); };

 private:
  TPinStateLog pinStateLog;

  String replaceLineBreakerToHtml(String input);
};
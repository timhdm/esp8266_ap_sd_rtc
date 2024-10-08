#pragma once
#include <Arduino.h>

#include <map>

#include "TPinStateLog.h"

class TPin {
 public:
  TPin();
  uint8_t convertPin(String pin);
  String getPinsStatusString();
  String getPinsStatusHtml();
  uint8_t getPin(String pin);
  uint8_t setPin(String pin, uint8_t value);
  boolean isOutput(String pin);
  boolean isInput(String pin) { return !isOutput(pin); };

 private:
  TPinStateLog pinStateLog;
};
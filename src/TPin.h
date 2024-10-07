#pragma once
#include <Arduino.h>

class TPin {
 public:
  static uint8_t getPinNumber(uint8_t pin);
  static String getPinsStatusString();
  static String getPinsStatusHtml();
  static void initPins();

 private:
};
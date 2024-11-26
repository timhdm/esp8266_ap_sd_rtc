/**
 * @file TSdCard.h
 * @brief  Class for working with SD card
 * @version 0.1.0
 * @date 17-10-2024
 */

#pragma once
#include "Arduino.h"
#include "SD.h"
#include "SPI.h"

class TSdCard {
 public:
  void begin();
  String read(const char *file_name);
  boolean append(const char *file_name, String message);
  boolean save(const char *file_name, const String &message);

 private:
  File open(const char *file_name, uint8_t mode);
};
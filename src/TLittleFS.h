/**
 * @file TLittleFS.h
 * @brief Functions for working with LittleFS
 * @version 0.1.0
 * @date 12-10-2024
 */
#pragma once

#include <Arduino.h>

#include "LittleFS.h"

class TLittleFS {
 public:
  void begin();
  void list_files();

 private:
  void list_dir(String path, int count = 0);
};
#pragma once

#include <Arduino.h>

#include "LittleFS.h"

class TLittleFS {
 public:
  void begin();
  void listFiles();

 private:
  void listDir(String path, int count = 0);
};
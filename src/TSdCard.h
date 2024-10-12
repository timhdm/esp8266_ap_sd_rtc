#pragma once
#include "Arduino.h"
#include "SD.h"
#include "SPI.h"

class TSdCard {
 public:
  TSdCard() : log_file(nullptr) {};
  void begin();
  void append(String file_name, String data);
  void list_files();

 private:
  File log_file;
};
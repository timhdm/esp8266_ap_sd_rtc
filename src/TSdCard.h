#pragma once
#include "Arduino.h"
#include "SD.h"
#include "SPI.h"

class TSdCard {
 public:
  TSdCard() : log_file(nullptr) {};
  void begin();
  void append(const char *file_name, String data);
  String read(const char *file_name, const size_t rows);
  void list_files();

 private:
  File log_file;
};
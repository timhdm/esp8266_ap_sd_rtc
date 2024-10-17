/**
 * @file TLog.h
 * @brief Логирование
 * @version 0.1.1
 * @date 17-10-2024
 */

#pragma once
#include <Arduino.h>
#include <TSdCard.h>
#include <TTime.h>

extern TTime time_now;

class TLog {
 public:
  TLog() : sd_card(nullptr), file_name("") {};

  void begin(TSdCard *sd_card, const char *file_name);
  void append(String log_string);

 private:
  TSdCard *sd_card;
  const char *file_name;
};

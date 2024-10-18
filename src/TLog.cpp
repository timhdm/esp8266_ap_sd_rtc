#include "TLog.h"

void TLog::begin(TSdCard* sd_card, const char* file_name) {
  this->sd_card = sd_card;
  this->file_name = file_name;
}

void TLog::append(String log_string) {
  if (this->file_name && this->sd_card != nullptr) {
    String buffer = time_now.fetch_now_string_short() + " " + log_string;
    this->sd_card->append(this->file_name, buffer);
    Serial.println("[LOG] '" + buffer + "' -> " + this->file_name);
  }
}

#include "TPinLog.h"
////////////////////////////////////////////////
//                 PINSTATE                   //
////////////////////////////////////////////////
TPinLog::PinState::PinState(String pin, uint8_t state)
    : pin(pin), state(state), timestamp(time_now.fetch_time_now_unix()) {}

String TPinLog::PinState::to_string() {
  struct tm *timeinfo = localtime(&timestamp);
  char time_buffer[80];
  strftime(time_buffer, sizeof(time_buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
  return String(time_buffer) + " Pin: " + String(pin) + " = " +
         String(state ? "ON" : "OFF") + "\n";
}

////////////////////////////////////////////////
//                  TPINLOG                   //
////////////////////////////////////////////////
String TPinLog::fetch(const size_t number) {
  String result = "";

  size_t count = pinLog.size() < number ? pinLog.size() : number;

  for (auto i = pinLog.size(); i > pinLog.size() - count; --i) {
    result += pinLog[i - 1].to_string();
  }

  return result;
}

void TPinLog::append(String pin, uint8_t state) {
  pinLog.push_back(
      PinState(pin, state));  // TODO: Хранить не более 10 элементов
  if (this->sd_log_file_name)
    this->sd_log_file.append(this->sd_log_file_name, pinLog.back().to_string());
}

String TPinLog::printTimestamp(time_t timestamp) {
  struct tm *timeinfo = localtime(&timestamp);
  char buffer[80];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  return String(buffer);
}

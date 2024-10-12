#include "TPin.h"
/*
 * Выводы требуют наличия определенного логического уровня на момент
 * включения микроконтроллера (OUTPUT): D0, D3, D4, D8
 * Выводы могут использоваться для получения сигнала (INPUT): D1, D2, D5, D6, D7
 */

////////////////////////////////////////////////
//                   TPIN                     //
////////////////////////////////////////////////
void TPin::begin() {
  for (uint8_t i = 0; i < 8; i++) {
    is_output("D" + String(i))
        ? pinMode(convert_string_pin("D" + String(i)), OUTPUT)
        : pinMode(convert_string_pin("D" + String(i)), INPUT);
  }
  this->pinlog.begin();
}

uint8_t TPin::get_pin(String pin) {
  return digitalRead(convert_string_pin(pin));
}

uint8_t TPin::set_pin(String pin, uint8_t value) {
  uint8_t returnError = 0;
  if (!is_output(pin)) {
    returnError = 1;
  } else {
    uint8_t pinConvertable = convert_string_pin(pin);
    if (pinConvertable != 255) {
      digitalWrite(pinConvertable, value);
      pinlog.append(pin, value);
    } else {
      returnError = 2;
    }
  }
  return returnError;
}

uint8_t TPin::convert_string_pin(String pin) {
  static const std::map<String, uint8_t> wemosD1PinMap = {
      {"D0", D0}, {"D1", D1}, {"D2", D2}, {"D3", D3}, {"D4", D4},
      {"D5", D5}, {"D6", D6}, {"D7", D7}, {"D8", D8}};
  auto it = wemosD1PinMap.find(pin);
  return it != wemosD1PinMap.end() ? it->second : 255;
}

boolean TPin::is_output(String pin) {
  static const std::map<String, uint8_t> wemosD1OutputPinMap = {
      {"D0", D0}, {"D3", D3}, {"D4", D4}, {"D8", D8}};

  auto it = wemosD1OutputPinMap.find(pin);

  return it != wemosD1OutputPinMap.end();
}

String TPin::get_pins_status(const bool html) {
  String result =
      "[A0:" + String(analogRead(A0)) + "]\n" +
      "[D0:" + String(digitalRead(D0)) + " D1:" + String(digitalRead(D1)) +
      " D2:" + String(digitalRead(D2)) + "]\n" +
      "[D3:" + String(digitalRead(D3)) + " D4:" + String(digitalRead(D4)) +
      " D5:" + String(digitalRead(D5)) + "]\n" +
      "[D6:" + String(digitalRead(D6)) + " D7:" + String(digitalRead(D7)) +
      " D8:" + String(digitalRead(D8)) + "]";

  return html ? replace_line_breaker_to_html(result) : result;
}

String TPin::get_pins_log(const int number, const bool html) {
  return html ? replace_line_breaker_to_html(pinlog.fetch(number))
              : pinlog.fetch(number);
}

String TPin::replace_line_breaker_to_html(String input) {
  input.replace("\n", "<br>");
  return input;
}

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
void TPinLog::begin() { sd_log_file.begin(); }

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

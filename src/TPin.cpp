#include "TPin.h"
/*
 * Выводы требуют наличия определенного логического уровня на момент
 * включения микроконтроллера (OUTPUT): D0, D3, D4, D8
 * Выводы могут использоваться для получения сигнала (INPUT): D1, D2, D5, D6, D7
 */

////////////////////////////////////////////////
//                   TPIN                     //
////////////////////////////////////////////////
TPin::TPin(std::vector<PinStatus> pins_d_mode)
    : pins_d_mode(pins_d_mode), pins_d_state(8, 0), pins_a_state(1, 0) {}

void TPin::begin(TSdCard *sd_log_file) {
  for (size_t i = 0; i < pins_d_mode.size(); i++) {
    this->pins_d_state[i] = digitalRead(convert_string_pin("D" + String(i)));
    switch (pins_d_mode[i]) {
      case PinStatus::INPUT_PIN:
        pinMode(convert_string_pin("D" + String(i)), INPUT);
        break;
      case PinStatus::OUTPUT_PIN:
        pinMode(convert_string_pin("D" + String(i)), OUTPUT);
        break;

      default:
        break;
    }
  }
  this->pin_log.begin(sd_log_file);
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
      pin_log.append(pin, value);
    } else {
      returnError = 2;
    }
  }
  return returnError;
}

const std::vector<uint8_t> &TPin::get_pins_d_state() {
  update_pins_d_state();
  return this->pins_d_state;
}

const std::vector<uint16_t> &TPin::get_pins_a_state() {
  update_pins_a_state();
  return this->pins_a_state;
}

void TPin::update_pins_d_state() {
  for (size_t i = 0; i < pins_d_mode.size(); i++) {
    this->pins_d_state[i] = digitalRead(convert_string_pin("D" + String(i)));
  }
}

void TPin::update_pins_a_state() {
  this->pins_a_state[0] = analogRead(convert_string_pin("A0"));
}

// TODO: Лучше возвращать словарь с статусами пинов, и всю информацию
// форматировать на стороне получателя
String TPin::fetch_pins_status(const bool as_html) {
  String result =
      "[A0:" + String(analogRead(A0)) + "]\n" +
      "[D0:" + String(digitalRead(D0)) + " D1:" + String(digitalRead(D1)) +
      " D2:" + String(digitalRead(D2)) + "]\n" +
      "[D3:" + String(digitalRead(D3)) + " D4:" + String(digitalRead(D4)) +
      " D5:" + String(digitalRead(D5)) + "]\n" +
      "[D6:" + String(digitalRead(D6)) + " D7:" + String(digitalRead(D7)) +
      " D8:" + String(digitalRead(D8)) + "]";

  return as_html ? replace_lb_to_br(result) : result;
}

String TPin::fetch_pins_log(const int rows_to_fetch, const bool as_html) {
  return as_html ? replace_lb_to_br(pin_log.fetch(rows_to_fetch))
                 : pin_log.fetch(rows_to_fetch);
}

boolean TPin::is_output(String pin) {
  String status = "";
  if (is_valid_pin(pin)) {
    PinStatus pin_number = pins_d_mode[pin.substring(1).toInt()];
    switch (pin_number) {
      case PinStatus::OUTPUT_PIN:
        status = " is output.";
        break;
      case PinStatus::INPUT_PIN:
        status = " is input.";
        break;
      default:
        status = " is undefined.";
        break;
    }
    Serial.println("[DBG] Pin " + pin + status);

    if (pin_number == PinStatus::OUTPUT_PIN) {
      return true;
    }
  }

  return false;
}

uint8_t TPin::convert_string_pin(String pin) {
  is_valid_pin(pin);
  static const std::map<String, uint8_t> wemosD1PinMap = {
      {"A0", A0}, {"D0", D0}, {"D1", D1}, {"D2", D2}, {"D3", D3},
      {"D4", D4}, {"D5", D5}, {"D6", D6}, {"D7", D7}, {"D8", D8}};
  auto it = wemosD1PinMap.find(pin);
  return it != wemosD1PinMap.end() ? it->second : 255;
}

String TPin::replace_lb_to_br(String input) {
  input.replace("\n", "<br>");
  return input;
}

bool TPin::is_valid_pin(String pin) {
  bool error = false;
  if (pin.length() < 2 || pin.length() > 3) {
    error = true;
  } else {
    if (pin[0] != 'D') {
      error = true;
    } else {
      switch (pin.length()) {
        case 2:
          error = isdigit(pin[1]) ? false : true;
          break;
        case 3:
          error = isdigit(pin[1]) && isdigit(pin[2]) ? false : true;
          break;

        default:
          break;
      }
    }
  }

  if (error) {
    Serial.println("[DBG] Invalid pin name!");
  }

  return !error;
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
  return String(time_buffer) + " [PIN] " + String(pin) + " = " +
         String(state ? "ON" : "OFF");
}

////////////////////////////////////////////////
//                  TPINLOG                   //
////////////////////////////////////////////////
void TPinLog::begin(TSdCard *sd_log_file) { this->sd_log_file = sd_log_file; }

String TPinLog::fetch(const size_t number) {
  String result = "";

  size_t count =
      pin_log_buffer.size() < number ? pin_log_buffer.size() : number;

  for (auto i = pin_log_buffer.size(); i > pin_log_buffer.size() - count; --i) {
    result += pin_log_buffer[i - 1].to_string() + "\n";
  }

  return result;
}

void TPinLog::append(String pin, uint8_t state) {
  pin_log_buffer.push_back(
      PinState(pin, state));  // TODO: Хранить не более 10 элементов
  if (this->sd_log_file_name && this->sd_log_file != nullptr)
    this->sd_log_file->append(this->sd_log_file_name,
                              pin_log_buffer.back().to_string());
}

String TPinLog::print_timestamp(time_t timestamp) {
  struct tm *timeinfo = localtime(&timestamp);
  char buffer[80];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  return String(buffer);
}

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
    this->pins_d_state[i] =
        digitalRead(get_pin_id_from_string("D" + String(i)));
    switch (pins_d_mode[i]) {
      case PinStatus::INPUT_PIN:
        pinMode(get_pin_id_from_string("D" + String(i)), INPUT);
        break;
      case PinStatus::OUTPUT_PIN:
        pinMode(get_pin_id_from_string("D" + String(i)), OUTPUT);
        break;

      default:
        break;
    }
  }
  this->pin_log.begin(sd_log_file);
}

uint8_t TPin::get_pin(String pin) {
  return digitalRead(get_pin_id_from_string(pin));
}

uint8_t TPin::set_pin(String pin, uint8_t value) {
  uint8_t returnError = 0;
  if (!is_output(pin)) {
    returnError = 1;
  } else {
    uint8_t pinConvertable = get_pin_id_from_string(pin);
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
    this->pins_d_state[i] =
        digitalRead(get_pin_id_from_string("D" + String(i)));
  }
}

void TPin::update_pins_a_state() {
  this->pins_a_state[0] = analogRead(get_pin_id_from_string("A0"));
}

String TPin::fetch_pins_log_buffer(const size_t rows, const bool as_html) {
  String log_buffer = pin_log.fetch(rows);
  return as_html ? replace_lb_to_br(log_buffer) : log_buffer;
}

String TPin::fetch_pins_log_sd(const size_t rows, const bool as_html) {
  String log_buffer = pin_log.fetch_sd(rows);
  return as_html ? replace_lb_to_br(log_buffer) : log_buffer;
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

uint8_t TPin::get_pin_id_from_string(String pin) {
  is_valid_pin(pin);
  static const std::map<String, uint8_t> wemosD1PinMap = {
      {"A0", A0}, {"D0", D0}, {"D1", D1}, {"D2", D2}, {"D3", D3},
      {"D4", D4}, {"D5", D5}, {"D6", D6}, {"D7", D7}, {"D8", D8}};
  auto it = wemosD1PinMap.find(pin);
  return it != wemosD1PinMap.end() ? it->second : 255;
}

bool TPin::is_valid_pin(String pin) {
  bool error = false;
  if (pin.length() < 2 || pin.length() > 3) {
    error = true;
  } else {
    if (pin[0] != 'D' && pin[0] != 'A') {
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
    Serial.println("[DBG] '" + String(pin) + "' Invalid pin name!");
  }

  return !error;
}

String TPin::replace_lb_to_br(String input) {
  input.replace("\n", "<br>");
  return input;
}

////////////////////////////////////////////////
//                 PINSTATE                   //
////////////////////////////////////////////////
TPinLog::PinState::PinState(String pin, uint8_t state)
    : pin(pin), state(state), timestamp(time_now.fetch_time_now_unix()) {}

String TPinLog::PinState::to_string() {
  return String(time_now.fetch_time_now_string_short()) + " [PIN] " +
         String(pin) + " = " + String(state ? "ON" : "OFF");
}

////////////////////////////////////////////////
//                  TPINLOG                   //
////////////////////////////////////////////////
void TPinLog::begin(TSdCard *sd_log_file) { this->sd_log_file = sd_log_file; }

String TPinLog::fetch(const size_t rows) {
  String result = "";

  size_t log_size = pin_log_buffer.size();
  size_t count = log_size < rows ? log_size : rows;

  for (auto i = log_size; i > log_size - count; --i) {
    if (i != log_size) result += "\n";
    result += pin_log_buffer[i - 1].to_string();
  }

  return result.length() > 0 ? result : "Empty log.";
}

String TPinLog::fetch_sd(const size_t rows) {
  return this->sd_log_file->read(this->sd_log_file_name, rows);
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

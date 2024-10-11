#include "TPin.h"
/*
 * Выводы требуют наличия определенного логического уровня на момент
 * включения микроконтроллера (OUTPUT): D0, D3, D4, D8
 * Выводы могут использоваться для получения сигнала (INPUT): D1, D2, D5, D6, D7
 */
TPin::TPin() {
  for (uint8_t i = 0; i < 8; i++) {
    isOutput("D" + String(i)) ? pinMode(convertPin("D" + String(i)), OUTPUT)
                              : pinMode(convertPin("D" + String(i)), INPUT);
  }
}

uint8_t TPin::getPin(String pin) { return digitalRead(convertPin(pin)); }

uint8_t TPin::setPin(String pin, uint8_t value) {
  uint8_t returnError = 0;
  if (!isOutput(pin)) {
    returnError = 1;
  } else {
    uint8_t pinConvertable = convertPin(pin);
    if (pinConvertable != 255) {
      digitalWrite(pinConvertable, value);
      pinStateLog.add(pin, value);
    } else {
      returnError = 2;
    }
  }
  return returnError;
}

uint8_t TPin::convertPin(String pin) {
  static const std::map<String, uint8_t> wemosD1PinMap = {
      {"D0", D0}, {"D1", D1}, {"D2", D2}, {"D3", D3}, {"D4", D4},
      {"D5", D5}, {"D6", D6}, {"D7", D7}, {"D8", D8}};
  auto it = wemosD1PinMap.find(pin);
  return it != wemosD1PinMap.end() ? it->second : 255;
}

boolean TPin::isOutput(String pin) {
  static const std::map<String, uint8_t> wemosD1OutputPinMap = {
      {"D0", D0}, {"D3", D3}, {"D4", D4}, {"D8", D8}};

  auto it = wemosD1OutputPinMap.find(pin);

  return it == wemosD1OutputPinMap.end() ? false : true;
}

String TPin::getPinsStatus(const bool html) {
  String result =
      "[A0:" + String(analogRead(A0)) + "]\n" +
      "[D0:" + String(digitalRead(D0)) + " D1:" + String(digitalRead(D1)) +
      " D2:" + String(digitalRead(D2)) + "]\n" +
      "[D3:" + String(digitalRead(D3)) + " D4:" + String(digitalRead(D4)) +
      " D5:" + String(digitalRead(D5)) + "]\n" +
      "[D6:" + String(digitalRead(D6)) + " D7:" + String(digitalRead(D7)) +
      " D8:" + String(digitalRead(D8)) + "]";

  return html ? replaceLineBreakerToHtml(result) : result;
}

String TPin::getPinStateLog(const int number, const bool html) {
  return html ? replaceLineBreakerToHtml(pinStateLog.getLast(number))
              : pinStateLog.getLast(number);
}

String TPin::replaceLineBreakerToHtml(String input) {
  input.replace("\n", "<br>");
  return input;
}
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

/**
 * @brief Convert a string pin name to a numeric pin value.
 * @details This function takes a string pin name (e.g. "D0", "D1", etc.) and
 *          returns the numeric value of the pin (e.g. 0, 1, etc.).
 * @param pin - string pin name (e.g. "D0", "D1", etc.)
 * @return numeric value of the pin, or 255 if the pin is not found.
 */
uint8_t TPin::convertPin(String pin) {
  static const std::map<String, uint8_t> wemosD1PinMap = {
      {"D0", D0}, {"D1", D1}, {"D2", D2}, {"D3", D3}, {"D4", D4},
      {"D5", D5}, {"D6", D6}, {"D7", D7}, {"D8", D8}};
  auto it = wemosD1PinMap.find(pin);
  return it != wemosD1PinMap.end() ? it->second : 255;
}

/**
 * @brief Return true if pin is an output pin, false otherwise.
 * @details D0, D3, D4 and D8 are the output pins.
 * @param pin - name of pin, e.g. "D0", "D2", ...
 * @return true if pin is an output pin, false otherwise
 */
boolean TPin::isOutput(String pin) {
  static const std::map<String, uint8_t> wemosD1OutputPinMap = {
      {"D0", D0}, {"D3", D3}, {"D4", D4}, {"D8", D8}};

  auto it = wemosD1OutputPinMap.find(pin);

  return it == wemosD1OutputPinMap.end() ? false : true;
}

/**
 * @brief Return current state of all pins as string.
 * @details This is useful for displaying the current state of all pins on a
 *          serial console or a web page.
 * @return Current state of all pins as string.
 */
String TPin::getPinsStatusString() {
  return "[A0:" + String(analogRead(A0)) + "]\n" +
         "[D0:" + String(digitalRead(D0)) + " D1:" + String(digitalRead(D1)) +
         " D2:" + String(digitalRead(D2)) + "]\n" +
         "[D3:" + String(digitalRead(D3)) + " D4:" + String(digitalRead(D4)) +
         " D5:" + String(digitalRead(D5)) + "]\n" +
         "[D6:" + String(digitalRead(D6)) + " D7:" + String(digitalRead(D7)) +
         " D8:" + String(digitalRead(D8)) + "]";
}

/**
 * @brief Return current state of all pins as HTML string.
 * @details This is useful for displaying the current state of all pins on a
 *          web page.
 * @return Current state of all pins as HTML string.
 */
String TPin::getPinsStatusHtml() {
  String result = getPinsStatusString();
  result.replace("\n", "<br>");
  return result;
}

/**
 * @brief Set the value of the specified pin.
 * @details This function sets the value of the specified pin to the specified
 *          value. If the pin is not an output pin, a non-zero error value is
 *          returned.
 * @param pin - name of pin, e.g. "D0", "D2", ...
 * @param value - value to set the pin to (0 or 1)
 * @return 0 if successful, 1 if pin is not an output pin, 2 if pin not found
 */
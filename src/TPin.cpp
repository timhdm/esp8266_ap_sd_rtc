#include "TPin.h"

/**
 * Получение номера пина по которому можно работать с пином в коде
 * @param pin номер на плате идущий после литеры D
 */
uint8_t TPin::getPinNumber(uint8_t pin) {
  switch (pin) {
    case 0:
      return D0;
    case 1:
      return D1;
    case 2:
      return D2;
    case 3:
      return D3;
    case 4:
      return D4;
    case 5:
      return D5;
    case 6:
      return D6;
    case 7:
      return D7;
    case 8:
      return D8;
    default:
      return 255;
  }
}

/**
 * Вывод состояния всех пинов в строку.
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
 * Вывод состояния всех пинов в формате HTML.
 */
String TPin::getPinsStatusHtml() {
  return "[A0:" + String(analogRead(A0)) + "]<br>" +
         "[D0:" + String(digitalRead(D0)) + " D1:" + String(digitalRead(D1)) +
         " D2:" + String(digitalRead(D2)) + "]<br>" +
         "[D3:" + String(digitalRead(D3)) + " D4:" + String(digitalRead(D4)) +
         " D5:" + String(digitalRead(D5)) + "]<br>" +
         "[D6:" + String(digitalRead(D6)) + " D7:" + String(digitalRead(D7)) +
         " D8:" + String(digitalRead(D8)) + "]";
}

/**
 * Блок инициализации пинов ввода/вывода.
 */
void TPin::initPins() {
  // Выводы которые требуют наличия определенного логического уровня на момент
  // включения микроконтроллера.
  pinMode(D0, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);  // LED
  pinMode(D8, OUTPUT);

  // Выводы которые могут использоваться для получения сигнала.
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);
}
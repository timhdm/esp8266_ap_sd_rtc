#include "TTime.h"

time_t TTime::getTime() {
  update();
  return unixTimeNow;
}

String TTime::getTimeString() {
  update();
  return asctime(&timeStructureNow);
}

time_t TTime::getOnlineSeconds() {
  update();
  return unixTimeNow - startTime;
}

String TTime::getOnlineString() {
  time_t timeDiff = getOnlineSeconds();
  uint16_t hour = timeDiff / 3600;
  uint8_t min = (timeDiff / 60) % 60;
  uint8_t sec = timeDiff % 60;
  return String(hour) + " hrs. " + String(min) + " min. " + String(sec) +
         " sec.";
}

/**
 * Обновление структуры c текущим временем.
 */
void TTime::update() {
  time(&unixTimeNow);
  localtime_r(&unixTimeNow, &timeStructureNow);
  if (startTime == 0 && unixTimeNow > 1700000000) {
    startTime = unixTimeNow;
  }
}
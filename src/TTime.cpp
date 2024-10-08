#include "TTime.h"

time_t TTime::getTime() {
  update();
  return unixTimeNow;
}

/**
 * Returns current time as a string in the format returned by asctime() function
 * (e.g. "Mon Jan 01 00:00:00 1900\n").
 */
String TTime::getTimeString() {
  update();
  return asctime(&timeStructureNow);
}

/**
 * Returns the number of seconds that have elapsed since the device
 * started.
 */
time_t TTime::getOnlineSeconds() {
  update();
  return unixTimeNow - startTime;
}

/**
 * Returns a string representation of the elapsed time since the device
 * started, in the format: "X hrs. Y min. Z sec.".
 */
String TTime::getOnlineString() {
  String hours, minutes, seconds;
  time_t timeDiff = getOnlineSeconds();
  uint16_t hour = timeDiff / 3600;
  uint8_t min = (timeDiff / 60) % 60;
  uint8_t sec = timeDiff % 60;
  hour < 10 ? hours = "0" + String(hour) : hours = String(hour);
  min < 10 ? minutes = "0" + String(min) : minutes = String(min);
  sec < 10 ? seconds = "0" + String(sec) : seconds = String(sec);
  return hours + ":" + minutes + ":" + seconds;
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
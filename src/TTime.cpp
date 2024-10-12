#include "TTime.h"

time_t TTime::fetch_time_now_unix() {
  update();
  return time_now_unix;
}

/**
 * Returns current time as a string in the format returned by asctime() function
 * (e.g. "Mon Jan 01 00:00:00 1900\n").
 */
String TTime::fetch_time_now_string() {
  update();
  return asctime(&time_now_structure);
}

String TTime::fetch_time_now_string_short() {
  char buffer[100];
  std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S",
                &time_now_structure);
  return String(buffer);
}

/**
 * Returns the number of seconds that have elapsed since the device
 * started.
 */
time_t TTime::fetch_online_seconds() {
  update();
  return time_now_unix - start_time;
}

/**
 * Returns a string representation of the elapsed time since the device
 * started, in the format: "X hrs. Y min. Z sec.".
 */
String TTime::fetch_online_string() {
  String hours, minutes, seconds;
  time_t timeDiff = fetch_online_seconds();
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
  time(&time_now_unix);
  localtime_r(&time_now_unix, &time_now_structure);
  if (start_time == 0 && time_now_unix > 1700000000) {
    start_time = time_now_unix;
  }
}
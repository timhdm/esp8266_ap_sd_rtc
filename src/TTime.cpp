#include "TTime.h"

const time_t TTime::fetch_unixtime() { return this->now_unixtime; }

String TTime::fetch_string_long() {
  this->update();
  struct tm *time_structure = localtime(&this->now_unixtime);
  return asctime(time_structure);
}

String TTime::fetch_string_short() {
  this->update();
  char buffer[100];
  struct tm *time_structure = localtime(&this->now_unixtime);
  std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", time_structure);
  return String(buffer);
}

String TTime::fetch_online_string() {
  String hours, minutes, seconds;
  time_t timeDiff = this->now_unixtime - this->start_unixtime;
  uint16_t hour = timeDiff / 3600;
  uint8_t min = (timeDiff / 60) % 60;
  uint8_t sec = timeDiff % 60;
  hour < 10 ? hours = "0" + String(hour) : hours = String(hour);
  min < 10 ? minutes = "0" + String(min) : minutes = String(min);
  sec < 10 ? seconds = "0" + String(sec) : seconds = String(sec);
  return hours + ":" + minutes + ":" + seconds;
}

void TTime::set_time(RtcDateTime time) {
  now_unixtime = time.Unix32Time();
  if (start_unixtime == 0) {
    start_unixtime = now_unixtime;
  }
}

void TTime::update() {
  if (millis() - previous_millis >= 1000) {
    previous_millis += 1000;
    this->now_unixtime += 1;
  }
}
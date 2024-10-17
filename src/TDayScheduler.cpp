#include "TDayScheduler.h"

void TDayScheduler::begin(const time_t& now_unixtime, uint16_t hour,
                          uint16_t minute, uint16_t duration_in_seconds) {
  set_duration(duration_in_seconds);
  set_schedule(now_unixtime, hour, minute);
}

void TDayScheduler::set_schedule(const time_t& now_unixtime, uint16_t hour,
                                 uint16_t minute) {
  // Преобразуем текущее время в структуру tm
  struct tm* timeinfo = localtime(&now_unixtime);

  // Меняем часы и минуты на переданные
  timeinfo->tm_hour = hour;
  timeinfo->tm_min = minute;
  timeinfo->tm_sec = 0;

  // Если новое вермя срабатывания в этих сутках уже прошло, то добавляем
  // сутки.
  next_unixtime = mktime(timeinfo);
  if (next_unixtime < now_unixtime) {
    next_unixtime += 86400;
  }
}
bool TDayScheduler::is_time(const time_t& now_unixtime) {
  if (enabled || duration_in_seconds == 0 || next_unixtime == 0) return false;
  bool return_flag = false;
  if (now_unixtime > next_unixtime) {
    return_flag = true;
    next_unixtime = now_unixtime + 86400;  // +1 сутки
  }
  return return_flag;
}

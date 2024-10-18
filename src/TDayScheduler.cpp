#include "TDayScheduler.h"

void TDayScheduler::set(uint8_t id, const time_t& now_unixtime, uint16_t hour,
                        uint16_t minute, uint16_t duration_in_seconds,
                        bool enabled) {
  this->id = id;
  this->enabled = enabled;
  this->duration_in_seconds = duration_in_seconds;
  this->hour = hour;
  this->minute = minute;

  // Преобразуем текущее время в структуру tm
  struct tm* timeinfo = localtime(&now_unixtime);

  // Меняем часы и минуты на переданные
  timeinfo->tm_hour = hour;
  timeinfo->tm_min = minute;
  timeinfo->tm_sec = 0;

  // Если новое вермя срабатывания в этих сутках уже прошло, то добавляем
  // сутки.
  start_unixtime = mktime(timeinfo);
  if (start_unixtime < now_unixtime) {
    start_unixtime += 86400;
  }

  end_unixtime = start_unixtime + duration_in_seconds;
}
bool TDayScheduler::is_up(const time_t& now_unixtime) {
  if (enabled || duration_in_seconds == 0 || start_unixtime == 0) return false;
  if (!up && now_unixtime > start_unixtime) {
    Serial.print(end_unixtime - now_unixtime);
    Serial.print(" ");
    Serial.println(duration_in_seconds * 0.9);
    up = true;
    start_unixtime = start_unixtime + 86400;  // +1 сутки
    log_system.append("Scheduler [" + String(this->id) + "] -> ON");
  }
  if (up && now_unixtime > end_unixtime) {
    up = false;
    end_unixtime =
        start_unixtime + 86400 + this->duration_in_seconds;  // +1 сутки
    log_system.append("Scheduler [" + String(this->id) + "] -> OFF");
  }
  return this->up;
}

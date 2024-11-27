#include "TDayScheduler.h"
/**
 * @brief Загружает данные расписания.
 * @param now_unixtime Текущее время в формате Unix.
 * @return Нет возвращаемого значения.
 *
 * Эта функция загружает данные расписания из настроек и устанавливает их для
 * каждого пина.
 */
void TDayScheduler::load() {
  for (uint8_t i = 0; i < PIN_MAX; i++) {
    enabled[i] = settings.getBoolean(String(i) + "_enabled", false);
    hour[i] = settings.getInt(String(i) + "_hour", 0);
    minute[i] = settings.getInt(String(i) + "_minute", 0);
    duration[i] = settings.getInt(String(i) + "_duration", 0);
  }

  this->log.append("[SHD] Scheduler load settings: " + to_string());
}

void TDayScheduler::save() {
  for (uint8_t i = 0; i < PIN_MAX; i++) {
    settings.set(String(i) + "_enabled", enabled[i]);
    settings.set(String(i) + "_hour", static_cast<uint32_t>(hour[i]));
    settings.set(String(i) + "_minute", static_cast<uint32_t>(minute[i]));
    settings.set(String(i) + "_duration", static_cast<uint32_t>(duration[i]));
  }

  settings.save();

  this->log.append("[SHD] Scheduler save settings: " + to_string());
}

void TDayScheduler::clean(uint8_t pin) { set_scheduler(pin, false, 0, 0, 0); }
void TDayScheduler::clean_all() {
  for (uint8_t i = 0; i < PIN_MAX; i++) {
    set_scheduler(i, false, 0, 0, 0);
  }
}

bool TDayScheduler::get_status(uint8_t pin, const time_t& now_unixtime) {
  if (pin >= PIN_MAX) {
    log.append("[SHD] Scheduler (get_status) error: " + String(pin) + " > " +
               String(PIN_MAX));
    return false; /* RETURN */
  }
  if (!is_duration_valid(duration[pin])) return false; /* RETURN */

  bool status = false;
  struct tm* timeinfo = localtime(&now_unixtime);
  uint16_t now = timeinfo->tm_hour * 60 + timeinfo->tm_min;
  uint16_t min = hour[pin] * 60 + minute[pin];
  uint16_t max = min + duration[pin];

  if (now >= min && now < max && enabled[pin]) {
    status = true;
  }

  return status;
}

void TDayScheduler::set_scheduler(uint8_t pin, bool enabled, uint16_t hour,
                                  uint16_t minute, uint16_t duration) {
  if (!is_duration_valid(duration) || pin >= PIN_MAX || hour > 23 ||
      minute > 59)
    return; /* RETURN */

  this->enabled[pin] = enabled;
  this->hour[pin] = hour;
  this->minute[pin] = minute;
  this->duration[pin] = duration;
}

////////////////////////////////////////////////
//               PRIVATE ZONE                 //
////////////////////////////////////////////////

boolean TDayScheduler::is_duration_valid(uint16_t duration) {
  bool status = true;
  if (duration > DURATION_MAX) {
    log.append("[SHD] Scheduler (get_status) error: duration=" + String() +
               ", duration must be <= " + String(DURATION_MAX));
    status = false;
  }
  return status;
}

String TDayScheduler::to_string() {
  String return_string = "";
  for (uint8_t i = 0; i < PIN_MAX; i++) {
    return_string += "[" + String(i) + "]: " + String(hour[i]) + ":" +
                     String(minute[i]) + " -> " + String(duration[i]) + " -> " +
                     String(enabled[i] ? "ON" : "OFF") + "; ";
  }
  return return_string;
}

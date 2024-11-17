#include "TDayScheduler.h"

bool TDayScheduler::get_status(const time_t& now_unixtime) {
  bool status = false;
  if (enabled || duration_in_seconds == 0 || start_unixtime == 0)
    return false;  ///// RETURN /////

  if (now_unixtime > start_unixtime) {
    if (now_unixtime < start_unixtime + duration_in_seconds) {
      status = true;
    } else {
      status = false;
      start_unixtime = start_unixtime + 86400;
    }
  }

  return status;
}

/**
 * @brief  Set new time for day scheduler
 * @param  id            Unique identifier of scheduler
 * @param  now_unixtime  Current unixtime
 * @param  hour          Hour of day when scheduler should start
 * @param  minute        Minute of day when scheduler should start
 * @param  duration_in_seconds   Duration of scheduler in seconds
 * @param  enabled       Whether scheduler is enabled or disabled
 */
void TDayScheduler::set(const time_t& now_unixtime, uint16_t hour,
                        uint16_t minute, uint16_t duration_in_seconds,
                        bool enabled) {
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
}

/**
 * @brief Load scheduler settings from EEPROM
 *
 * This function loads scheduler settings from EEPROM and initialize class
 * members. This function should be called in setup() function.
 */
void TDayScheduler::load(const time_t& now_unixtime) {
  this->log.append("[SHD] Scheduler current settings: " + to_string());

  uint8_t _hour =
      preferences.getUChar((String(this->pin) + "_hour").c_str(), 0);
  uint8_t _minute =
      preferences.getUChar((String(this->pin) + "_minute").c_str(), 0);
  uint32_t _duration_in_seconds =
      preferences.getULong((String(this->pin) + "_duration").c_str(), 0);
  bool _enabled =
      preferences.getBool((String(this->pin) + "_enabled").c_str(), 0);

  set(now_unixtime, _hour, _minute, _duration_in_seconds, _enabled);

  this->log.append("[SHD] Scheduler load settings: " + to_string());
}

/**
 * @brief Save scheduler settings to EEPROM
 *
 * This function saves scheduler settings to EEPROM. Before saving settings,
 * function checks validity of hour and minute values. If values are invalid,
 * function writes error message to log and does not save settings.
 */
void TDayScheduler::save() {
  if (is_hour_valid(this->hour) && is_minutes_valid(this->minute)) {
    preferences.putUChar((String(pin) + "_hour").c_str(), this->hour);
    preferences.putUChar((String(pin) + "_minute").c_str(), this->minute);
    preferences.putULong((String(pin) + "_duration").c_str(),
                         this->duration_in_seconds);
    preferences.putBool((String(pin) + "_enabled").c_str(), this->enabled);

    this->log.append("[SHD] Scheduler save: " + to_string());
  } else {
    this->log.append("[SHD] Save scheduler error. Value error: hour(" +
                     String(hour) + "), minute(" + String(minute) + ")");
  }
}

bool TDayScheduler::is_hour_valid(uint8_t hour) {
  return hour >= 0 && hour <= 23;
}

bool TDayScheduler::is_minutes_valid(uint8_t minute) {
  return minute >= 0 && minute <= 59;
}

/**
 * @brief Check if the duration in seconds is valid
 * @param duration_in_seconds Duration in seconds
 * @return true if duration is more then 1 second and less then 12 hours, false
 * otherwise
 */
bool TDayScheduler::is_duration_valid(uint8_t duration_in_seconds) {
  return duration_in_seconds >= 1 && duration_in_seconds < 60 * 60 * 12;
}

String TDayScheduler::to_string() {
  return ("[" + String(this->pin) + "] " + String(this->hour) + ":" +
          String(this->minute) + " - " + String(this->duration_in_seconds)) +
         "sec. " + String(this->enabled ? "ON" : "OFF");
}
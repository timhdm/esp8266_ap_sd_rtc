#include "TTime.h"
bool TTime::wasError(const char* errorTopic) {
  uint8_t error = this->rtc.LastError();
  if (error != 0) {
    // we have a communications error
    // see
    // https://www.arduino.cc/reference/en/language/functions/communication/wire/endtransmission/
    // for what the number means
    Serial.print("[");
    Serial.print(errorTopic);
    Serial.print("] WIRE communications error (");
    Serial.print(error);
    Serial.print(") : ");

    switch (error) {
      case Rtc_Wire_Error_None:
        Serial.println("(none?!)");
        break;
      case Rtc_Wire_Error_TxBufferOverflow:
        Serial.println("transmit buffer overflow");
        break;
      case Rtc_Wire_Error_NoAddressableDevice:
        Serial.println("no device responded");
        break;
      case Rtc_Wire_Error_UnsupportedRequest:
        Serial.println("device doesn't support request");
        break;
      case Rtc_Wire_Error_Unspecific:
        Serial.println("unspecified error");
        break;
      case Rtc_Wire_Error_CommunicationTimeout:
        Serial.println("communications timed out");
        break;
    }
    return true;
  }
  return false;
}

void TTime::begin() {
  this->rtc.Begin(D2, D1);

  if (!this->rtc.IsDateTimeValid()) {
    if (!wasError("setup IsDateTimeValid")) {
      // Common Causes:
      //    1) first time you ran and the device wasn't running yet
      //    2) the battery on the device is low or even missing

      Serial.println("RTC lost confidence in the DateTime!");
      // following line sets the RTC to the date & time this sketch was compiled
      // it will also reset the valid flag internally unless the Rtc device is
      // having an issue

      this->rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__));
    }
  }

  if (!this->rtc.GetIsRunning()) {
    if (!wasError("setup GetIsRunning")) {
      Serial.println("RTC was not actively running, starting now");
      this->rtc.SetIsRunning(true);
    }
  }

  this->start_time = this->update();
}

time_t TTime::fetch_time_now_unix() { return this->update(); }

/**
 * Returns current time as a string in the format returned by asctime() function
 * (e.g. "Mon Jan 01 00:00:00 1900\n").
 */
String TTime::fetch_time_now_string() {
  this->update();
  return asctime(&this->time_now_structure);
}

String TTime::fetch_time_now_string_short() {
  this->update();
  char buffer[100];
  std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S",
                &this->time_now_structure);
  return String(buffer);
}

/**
 * Returns the number of seconds that have elapsed since the device
 * started.
 */
time_t TTime::fetch_online_seconds() {
  return this->update() - this->start_time;
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

void TTime::set_time(RtcDateTime time) { this->rtc.SetDateTime(time); }

/**
 * Обновление структуры c текущим временем.
 */
time_t TTime::update() {
  this->time_now_unix = this->rtc.GetDateTime().Unix32Time();
  localtime_r(&this->time_now_unix, &this->time_now_structure);
  return this->time_now_unix;
}
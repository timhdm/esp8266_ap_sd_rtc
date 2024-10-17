#include "TTime.h"
bool TTime::wasError(const char* errorTopic) {
  uint8_t error = this->rtc.LastError();
  if (error != 0) {
    Serial.print("[" + String(errorTopic) + "] WIRE communications error (" +
                 String(error) + ") : ");

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
      // Общие причины:
      // 1) вы запустили устройство впервые и оно еще неработало
      // 2) батарея на устройстве разряжена или вообще отсутствует

      Serial.println("RTC lost confidence in the DateTime!");
      // следующая строка устанавливает RTC на дату и время, когда этот скетч
      // был скомпилирован она также сбросит флаг valid внутри, если только
      // устройство Rtc не имеет проблемы

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

const time_t TTime::fetch_time_now_unix() { return this->update(); }

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

time_t TTime::fetch_online_seconds() {
  return this->update() - this->start_time;
}

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

time_t TTime::update() {
  this->time_now_unix = this->rtc.GetDateTime().Unix32Time();
  localtime_r(&this->time_now_unix, &this->time_now_structure);
  return this->time_now_unix;
}
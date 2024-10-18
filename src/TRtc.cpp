#include "TRtc.h"
bool TRtc::wasError(const char* errorTopic) {
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

void TRtc::begin(int sda, int scl) {
  this->rtc.Begin(sda, scl);

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
}

RtcDateTime TRtc::getDateTime() { return this->rtc.GetDateTime(); }

void TRtc::set_time(RtcDateTime time) { this->rtc.SetDateTime(time); }

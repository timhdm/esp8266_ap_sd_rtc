#include "main.h"

// AP IP address: 192.168.4.1
TTime time_now;
TWeb web;
Preferences preferences;
Preferences preferences_scheduler;
TLittleFS little_fs;
TSdCard sd_card;
TLog log_system;
TPin pins({PinStatus::OUTPUT_PIN,     // D0
           PinStatus::OUTPUT_PIN,     // D1
           PinStatus::UNDEFINED_PIN,  // D2
           PinStatus::UNDEFINED_PIN,  // D3
           PinStatus::OUTPUT_PIN,     // D4
           PinStatus::UNDEFINED_PIN,  // D5
           PinStatus::UNDEFINED_PIN,  // D6
           PinStatus::UNDEFINED_PIN,  // D7
           PinStatus::UNDEFINED_PIN}  // D8
);
GTimer timer_1s(MS);
TDayScheduler scheduler_pin_d0(0, log_system, preferences_scheduler);
TDayScheduler scheduler_pin_d1(1, log_system, preferences_scheduler);
TDayScheduler scheduler_pin_d4(4, log_system, preferences_scheduler);
TRtc rtc;

void setup() {
  Serial.begin(115200);
  Serial.println("\n[SYS] Initializing...");

  preferences.begin("system");
  preferences_scheduler.begin("scheduler");
  little_fs.begin();
  sd_card.begin();
  pins.begin(&sd_card);
  log_system.begin(&sd_card, "system.log");

  timer_1s.setInterval(1000);

  rtc.begin(D3, D2);
  time_now.set_time(rtc.getDateTime());

  // TODO
  scheduler_pin_d0.set(time_now.fetch_unixtime(), 16, 50, 30);
  scheduler_pin_d1.set(time_now.fetch_unixtime(), 16, 51, 21);
  scheduler_pin_d4.set(time_now.fetch_unixtime(), 16, 52, 10);
  scheduler_pin_d0.save();
  scheduler_pin_d1.save();
  scheduler_pin_d4.save();

  String ssid = getSsid();
  const char* password = "esp12345";  // TODO
  WiFi.softAP(ssid, password);
  Serial.println("[SYS] Access point: " + ssid);
  Serial.println("[SYS] IP address: " + WiFi.softAPIP().toString());

  web.begin();
}

void loop() {
  time_now.update();
  if (timer_1s.isReady()) loop_scheduler(time_now.fetch_unixtime());
}

////////////////////////////////////////////////
//                 FUNCTIONS                  //
////////////////////////////////////////////////
void loop_scheduler(const time_t& now_unixtime) {
  bool d0_schedule_status = scheduler_pin_d0.get_status(now_unixtime);
  if (pins.get_pin("D0") != d0_schedule_status)
    pins.set_pin("D0", d0_schedule_status);

  bool d1_schedule_status = scheduler_pin_d1.get_status(now_unixtime);
  if (pins.get_pin("D1") != d1_schedule_status)
    pins.set_pin("D1", d1_schedule_status);

  bool d4_schedule_status = scheduler_pin_d4.get_status(now_unixtime);
  if (pins.get_pin("D4") != d4_schedule_status)
    pins.set_pin("D4", d4_schedule_status);
}

String getSsid() {
  String ssid = preferences.getString("ssid", "NONE");
  if (ssid == "NONE") {
    String suffix = WiFi.macAddress();
    suffix.replace(":", "");
    ssid = "ESP8266_" + suffix.substring(suffix.length() - 4);
    preferences.putString("ssid", ssid);
  }
  return ssid;
}

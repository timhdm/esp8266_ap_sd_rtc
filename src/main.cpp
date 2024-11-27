#include "main.h"

// AP IP address: 192.168.4.1
TTime time_now;
TWeb web;
Preferences preferences;
TLittleFS little_fs;
TSdCard sd_card;
TSettings settings(&sd_card);
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
TDayScheduler scheduler(log_system, settings);
TRtc rtc;

void setup() {
  Serial.begin(115200);
  Serial.println("\n[SYS] Initializing...");

  preferences.begin("system");
  little_fs.begin();
  sd_card.begin();
  settings.begin();
  pins.begin(&sd_card);
  log_system.begin(&sd_card, "system.log");

  timer_1s.setInterval(1000);

  rtc.begin(D3, D2);
  time_now.set_time(rtc.getDateTime());

  // TODO debug, remove later
  time_t now = time_now.fetch_unixtime();
  struct tm* timeinfo = localtime(&now);
  int hour_ = timeinfo->tm_hour;   // час (0-23)
  int minute_ = timeinfo->tm_min;  // минута (0-59)
  pins.set_pin("D0", false);
  pins.set_pin("D1", false);
  scheduler.load();
  scheduler.set_scheduler(0, true, hour_, minute_ + 1, 3);
  scheduler.set_scheduler(1, true, hour_, minute_ + 2, 1);
  scheduler.save();

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
  boolean d0_schedule_status = scheduler.get_status(0, now_unixtime);
  if (pins.get_pin("D0") != d0_schedule_status)
    pins.set_pin("D0", d0_schedule_status);

  boolean d1_schedule_status = scheduler.get_status(1, now_unixtime);
  if (pins.get_pin("D1") != d1_schedule_status)
    pins.set_pin("D1", d1_schedule_status);

  Serial.printf("%s D0: %d, D1: %d\n", time_now.fetch_string_short(),
                d0_schedule_status,
                d1_schedule_status);  // TODO debug, remove later
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

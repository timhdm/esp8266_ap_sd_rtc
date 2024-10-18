#include "main.h"

// AP IP address: 192.168.4.1
TTime time_now;
TWeb web;
Preferences preferences;
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
std::array<TDayScheduler, 8> scheduler_pool{};
TRtc rtc;

void setup() {
  Serial.begin(115200);
  Serial.println("\n[SYS] Initializing...");

  preferences.begin("esp8266");
  little_fs.begin();
  sd_card.begin();
  pins.begin(&sd_card);
  log_system.begin(&sd_card, "system.log");

  timer_1s.setInterval(1000);

  rtc.begin(D3, D2);
  time_now.set_time(rtc.getDateTime());

  // TODO
  scheduler_pool[0].set(0, time_now.fetch_now_unixtime(), 11, 43, 30);
  scheduler_pool[3].set(3, time_now.fetch_now_unixtime(), 11, 44, 21);
  scheduler_pool[4].set(4, time_now.fetch_now_unixtime(), 11, 45, 10);

  String ssid = getSsid();
  const char* password = "esp12345";  // TODO
  WiFi.softAP(ssid, password);
  Serial.println("[SYS] Access point: " + ssid);
  Serial.println("[SYS] IP address: " + WiFi.softAPIP().toString());

  web.begin();
}

void loop() {
  time_now.update();
  if (timer_1s.isReady()) loop_scheduler(time_now.fetch_now_unixtime());
}

////////////////////////////////////////////////
//                 FUNCTIONS                  //
////////////////////////////////////////////////
void loop_scheduler(const time_t& now_unixtime) {
  scheduler_pool[0].is_up(now_unixtime);
  scheduler_pool[3].is_up(now_unixtime);
  scheduler_pool[4].is_up(now_unixtime);
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

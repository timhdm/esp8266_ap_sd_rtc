#include "main.h"

// AP IP address: 192.168.4.1
TTime time_now;
TWeb web;
Preferences preferences;
TLittleFS little_fs;
TSdCard sd_card;
TLog log_system;
TPin pins({PinStatus::OUTPUT_PIN,     // D0
           PinStatus::UNDEFINED_PIN,  // D1
           PinStatus::UNDEFINED_PIN,  // D2
           PinStatus::OUTPUT_PIN,     // D3
           PinStatus::OUTPUT_PIN,     // D4
           PinStatus::UNDEFINED_PIN,  // D5
           PinStatus::UNDEFINED_PIN,  // D6
           PinStatus::UNDEFINED_PIN,  // D7
           PinStatus::UNDEFINED_PIN}  // D8
);
std::array<TDayScheduler, 8> scheduler_pool{};

void setup() {
  Serial.begin(115200);
  Serial.println("\n[SYS] Initializing...");

  time_now.begin();
  preferences.begin("esp8266");
  little_fs.begin();
  sd_card.begin();
  pins.begin(&sd_card);
  log_system.begin(&sd_card, "system.log");

  // TODO
  scheduler_pool[0].set(0, time_now.fetch_time_now_unix(), 20, 52, 1);
  scheduler_pool[3].set(3, time_now.fetch_time_now_unix(), 20, 53, 1);
  scheduler_pool[4].set(4, time_now.fetch_time_now_unix(), 20, 54, 1);

  String ssid = getSsid();
  const char* password = "esp12345";  // TODO
  WiFi.softAP(ssid, password);
  Serial.println("[SYS] Access point: " + ssid);
  Serial.println("[SYS] IP address: " + WiFi.softAPIP().toString());

  web.begin();
}

void loop() {
  scheduler_pool[0].is_up(time_now.fetch_time_now_unix());
  scheduler_pool[3].is_up(time_now.fetch_time_now_unix());
  scheduler_pool[4].is_up(time_now.fetch_time_now_unix());
}

////////////////////////////////////////////////
//                 FUNCTIONS                  //
////////////////////////////////////////////////
void loop_scheduler() {
  for (size_t i = 0; i < scheduler_pool.size(); ++i) {
    scheduler_pool[i].is_up(time_now.fetch_time_now_unix());
  }
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

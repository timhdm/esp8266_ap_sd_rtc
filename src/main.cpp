#include "main.h"

// AP IP address: 192.168.4.1
TTime time_now;
TWeb web;
Preferences preferences;
TLittleFS little_fs;
TSdCard sd_card;
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
GTimer timer_one_minute(MS, 60000);

void setup() {
  Serial.begin(115200);
  Serial.println("\n[SYS] Initializing...");

  time_now.begin();
  preferences.begin("esp8266");
  little_fs.begin();
  sd_card.begin();
  pins.begin(&sd_card);

  String ssid = getSsid();
  const char* password = "esp12345";  // TODO
  WiFi.softAP(ssid, password);
  Serial.println("[SYS] Access point: " + ssid);
  Serial.println("[SYS] IP address: " + WiFi.softAPIP().toString());

  web.begin();
}

void loop() {
  if (timer_one_minute.isReady()) trigger_one_minute();
}

////////////////////////////////////////////////
//                 FUNCTIONS                  //
////////////////////////////////////////////////
/**
 * Блок инструкций для запуска раз в секунду.
 */
void trigger_one_minute() {}

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

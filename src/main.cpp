#include "main.h"

// AP IP address: 192.168.4.1
TTime time_now;
TWeb web;
Preferences preferences;
TLittleFS little_fs;
TPin pins;
GTimer timer_one_second(MS, 1000);

void setup() {
  Serial.begin(115200);
  Serial.println("\n[SYS] Initializing...");

  preferences.begin("esp8266");
  little_fs.begin();
  pins.begin();

  String ssid = getSsid();
  const char* password = "esp12345";  // TODO
  WiFi.softAP(ssid, password);
  Serial.println("[SYS] Access point: " + ssid);
  Serial.println("[SYS] IP address: " + WiFi.softAPIP().toString());

  web.begin();
}

void loop() {
  if (timer_one_second.isReady()) triggerOneSecond();
}
////////////////////////////////////////////////
//                 FUNCTIONS                  //
////////////////////////////////////////////////
/**
 * Блок инструкций для запуска раз в секунду.
 */
void triggerOneSecond() { digitalWrite(D4, !digitalRead(D4)); }

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

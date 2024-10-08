#include "main.h"

// AP IP address: 192.168.4.1
TTime timeNow;
TWeb web;
Preferences preferences;
TLittleFS littleFS;
TPin pins;
GTimer timerOneSecond(MS, 1000);

void setup() {
  Serial.begin(115200);
  Serial.println("\nInitializing...");

  preferences.begin("esp8266");
  littleFS.begin();

  String ssid = getSsid();
  const char* password = "esp12345";  // TODO
  WiFi.softAP(ssid, password);
  Serial.println("Access point: " + ssid);
  Serial.println("IP address: " + WiFi.softAPIP().toString());

  web.begin();
}

void loop() {
  if (timerOneSecond.isReady()) triggerOneSecond();
}

// **************************************************
// ******************* FUNCTIONS ********************
// **************************************************
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

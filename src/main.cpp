#include "main.h"

// AP IP address: 192.168.4.1
GTimer timerOneSecond(MS);
struct tm timeStructureNow;
TWeb web;
TTime timeNow;
TSettings settings;

void setup() {
  Serial.begin(115200);
  Serial.println("\nInitializing...");

  if (LittleFS.begin()) {
    listFiles();
  } else {
    Serial.println(F("*FS: An Error has occurred while mounting LittleFS."));
  }

  preferences.begin("my-app");
  TPin::initPins();

  String ssid = getSsid();
  const char* password = "esp12345";  // TODO

  WiFi.softAP(ssid, password);

  Serial.println("Access point: " + ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Serial.printf("*NTP: Synchronization");
  // for (int i = 0; i < 10; ++i) {
  //   if (time(nullptr) < 1700000000) {
  //     Serial.printf(".");
  //     delay(1000);
  //   } else {
  //     Serial.printf(" [OK]\n*NTP: %s", timeNow.getTimeString().c_str());
  //     break;
  //   }
  // }

  web.begin();
}

// **************************************************
// ********************* LOOP ***********************
// **************************************************
void loop() {
  if (timerOneSecond.isReady()) triggerOneSecond();
}

/**
 * Блок инструкций для запуска раз в секунду.
 */
void triggerOneSecond() {
  // digitalWrite(D4, !digitalRead(D4));
}

String getSsid() {
  String ssid = settings.getString("ssid", String(0));
  if (ssid == "0") {
    String suffix = WiFi.macAddress();
    suffix.replace(":", "");
    ssid = "ESP8266_" + suffix.substring(suffix.length() - 4);
    setings.putString("ssid", ssid);
  }
  return ssid;
}

void listFiles() {
  Serial.println(F("Список файлов в LittleFS:"));

  Dir dir = LittleFS.openDir("/");

  Serial.println(F("--- LittleFS files: ---"));
  while (dir.next()) {
    Serial.println(dir.fileName());
  }
  Serial.println(F("-----------------------"));
}
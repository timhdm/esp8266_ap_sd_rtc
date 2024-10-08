#include "main.h"

// AP IP address: 192.168.4.1
GTimer timerOneSecond(MS);
struct tm timeStructureNow;
TWeb web;
TTime timeNow;
Preferences preferences;

void setup() {
  Serial.begin(115200);
  Serial.println("\nInitializing...");

  preferences.begin("esp8266");

  if (LittleFS.begin()) {
    listFiles();
  } else {
    Serial.println(F("*FS: An Error has occurred while mounting LittleFS."));
  }

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

void listDir(String path, int count = 0) {
  Dir dir = LittleFS.openDir(path);

  while (dir.next()) {
    String fileName = dir.fileName();
    File file = dir.openFile("r");

    String divider = "";
    for (int i = 0; i < count; i++) {
      divider += "*";
    }

    if (file.isDirectory()) {
      Serial.println(divider + F("[DIR] ") + fileName);
      listDir(fileName, count + 1);
    } else {
      Serial.println(divider + F("[FILE] ") + fileName);
    }

    file.close();
  }
}

void listFiles() {
  Serial.println(F("LittleFS:"));
  listDir("/");  // Старт с корневой директории
}
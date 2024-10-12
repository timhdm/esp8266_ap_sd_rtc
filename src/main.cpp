#include "main.h"

// AP IP address: 192.168.4.1
TTime time_now;
TWeb web;
Preferences preferences;
TLittleFS little_fs;
TPin pins;
GTimer timer_one_second(MS, 1000);
TSdCard sd_log_file;

void setup() {
  Serial.begin(115200);
  Serial.println("\nInitializing...");

  preferences.begin("esp8266");
  little_fs.begin();
  sd_log_file.begin();

  String ssid = getSsid();
  const char* password = "esp12345";  // TODO
  WiFi.softAP(ssid, password);
  Serial.println("Access point: " + ssid);
  Serial.println("IP address: " + WiFi.softAPIP().toString());

  web.begin();
}

void loop() {
  if (timer_one_second.isReady()) triggerOneSecond();
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

// void setup() {
//   Serial.begin(115200);
//   Serial.print("Initializing SD card: ");

//   while (!SD.begin(D8)) {
//     Serial.println(".");
//     delay(3000);
//   }
//   Serial.println("[OK]");

//   myFile = SD.open("test.txt", FILE_WRITE);

//   if (myFile) {
//     Serial.print("Writing to test.txt...");
//     myFile.println("testing 1, 2, 3.");
//     myFile.close();
//     Serial.println("done.");
//   } else {
//     Serial.println("error opening test.txt");
//   }

//   myFile = SD.open("test.txt");
//   if (myFile) {
//     Serial.println("test.txt:");

//     while (myFile.available()) {
//       Serial.write(myFile.read());
//     }
//     myFile.close();
//   } else {
//     Serial.println("error opening test.txt");
//   }
// }
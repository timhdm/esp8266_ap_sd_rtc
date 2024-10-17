#include "TSdCard.h"

void TSdCard::begin() {
  Serial.print("[SDC] Initializing SD card: ");

  for (uint8_t count = 0; count < 10; count++) {
    SD.end();
    if (!SD.begin(D8)) {
      Serial.println(
          "[SDC] An Error has occurred while mounting SD card. Retrying [" +
          String(count) + "]...");
      delay(1000);
    } else {
      Serial.println("[OK]");
      break;
    }
  }
}

void TSdCard::append(const char *file_name, String message) {
  this->log_file = SD.open(file_name, FILE_WRITE);
  if (this->log_file) {
    message.replace("\n", "");
    Serial.print("[SDC] Append [" + message + "] to file " + file_name + "...");
    this->log_file.println(message);
    this->log_file.close();
    Serial.println("[OK]");
  }
}

String TSdCard::read(const char *file_name, const size_t rows) {
  File logFile = SD.open(file_name, FILE_READ);
  if (!logFile) {
    Serial.println("[SDC] Failed to open log file.");
    return "[SDC] Unable to open log file.";
  }

  String logs = "";
  size_t counter = 0;
  while (logFile.available()) {
    Serial.println(String(counter) + " - " + String(rows));
    if (rows != 0 && counter > rows) break;
    logs += logFile.readStringUntil('\n');
    logs += "\n";
    counter++;
  }

  logFile.close();
  return logs;
}

void TSdCard::list_files() {}

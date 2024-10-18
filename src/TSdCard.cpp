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
    this->log_file.println(message);
    this->log_file.close();
  }
}

String TSdCard::read(const char *file_name, const size_t rows) {
  File file = SD.open(file_name, FILE_READ);
  if (!file) {
    String error_message = "[SDC] Unable to open file: " + String(file_name);
    Serial.println(error_message);
    return error_message;
  }

  String content = "";
  size_t counter = 0;
  while (file.available()) {
    if (rows != 0 && counter > rows) break;
    content += file.readStringUntil('\n');
    content += "\n";
    counter++;
  }

  file.close();
  Serial.println(String(file_name) + ": " + content);
  return content;
}

void TSdCard::list_files() {}

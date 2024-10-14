#include "TSdCard.h"

void TSdCard::begin() {
  Serial.print("[SDC] Initializing SD card: ");

  for (uint8_t count = 0; count < 10; count++) {
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

void TSdCard::append(String file_name, String message) {
  this->log_file = SD.open(file_name, FILE_WRITE);
  if (this->log_file) {
    message.replace("\n", "");
    Serial.print("[SDC] Append [" + message + "] to file " + file_name + "...");
    this->log_file.println(message);
    this->log_file.close();
    Serial.println("[OK]");
  }
}

void TSdCard::list_files() {}

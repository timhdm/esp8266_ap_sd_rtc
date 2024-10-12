#include "TSdCard.h"

void TSdCard::begin() {
  Serial.print("[SD] Initializing SD card: ");

  for (uint8_t count = 0; count < 10; count++) {
    if (!SD.begin(D8)) {
      Serial.println(
          "[SD] An Error has occurred while mounting SD card. Retrying [" +
          String(count) + "]...");
      delay(1000);
    } else {
      Serial.println("SD card mounted.");
      break;
    }
  }
}

void TSdCard::append(String file_name, String data) {
  this->log_file = SD.open(file_name, FILE_WRITE);
  if (this->log_file) {
    Serial.print("[SD] Append [" + data + "] to file " + file_name + "...");
    this->log_file.println(data);
    this->log_file.close();
    Serial.println("done.");
  }
}

void TSdCard::list_files() {}

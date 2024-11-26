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

String TSdCard::read(const char *file_name) {
  File file = SD.open(file_name, FILE_READ);
  if (!file) {
    String error_message = "[SDC] Unable to open file: " + String(file_name);
    Serial.println(error_message);
    return error_message;
  }

  String content = "";
  size_t counter = 0;
  while (file.available()) {
    content += file.readStringUntil('\n');
    content += "\n";
    counter++;
  }

  file.close();
  Serial.printf("[SDC] Read %s: %s\n", String(file_name).c_str(),
                content.c_str());
  return content;
}

boolean TSdCard::append(const char *file_name, String message) {
  File file = open(file_name, FILE_WRITE);
  boolean error = boolean(!file);

  if (!error) {
    message.replace("\n", "");
    file.println(message);
    file.close();
  }

  return error;
}

boolean TSdCard::save(const char *file_name, const String &message) {
  File file = open(file_name, FILE_WRITE);
  boolean error = boolean(!file);

  if (!error) {
    file.truncate(0);
    size_t bytesWritten = file.print(message);

    if (bytesWritten == 0) {
      error = true;
      Serial.println("[SDC] Error writing data to a file.");
    } else {
      Serial.printf(
          "[SDC] The file has been successfully saved. Bytes written: %d\n",
          bytesWritten);
    }
    file.close();
  }

  return error;
}

File TSdCard::open(const char *file_name, uint8_t mode) {
  File file = SD.open(file_name, mode);
  if (!file) {
    String error_message = "[SDC] Unable to open file: " + String(file_name);
    Serial.println(error_message);
  }
  return file;
}

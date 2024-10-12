#include "TLittleFS.h"

void TLittleFS::begin() {
  if (LittleFS.begin()) {
    list_files();
  } else {
    Serial.println(F("*FS: An Error has occurred while mounting LittleFS."));
  }
}

void TLittleFS::list_dir(String path, int count) {
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
      list_dir(fileName, count + 1);
    } else {
      Serial.println(divider + F("[FILE] ") + fileName);
    }

    file.close();
  }
}

void TLittleFS::list_files() {
  Serial.println(F("LittleFS:"));
  list_dir("/");  // Старт с корневой директории
}
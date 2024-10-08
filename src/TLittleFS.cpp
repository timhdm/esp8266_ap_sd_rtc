#include "TLittleFS.h"

void TLittleFS::begin() {
  if (LittleFS.begin()) {
    listFiles();
  } else {
    Serial.println(F("*FS: An Error has occurred while mounting LittleFS."));
  }
}

void TLittleFS::listDir(String path, int count) {
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

void TLittleFS::listFiles() {
  Serial.println(F("LittleFS:"));
  listDir("/");  // Старт с корневой директории
}
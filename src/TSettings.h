#pragma once
#include <Arduino.h>
#include <LittleFS.h>

#include <map>
#include <string>

class TSettings {
 public:
  TSettings();  // Конструктор для загрузки настроек
  String getValue(
      const char *key,
      const char *defaultValue = "NONE");  // Получение значения по ключу
  void setValue(const char *key,
                const char *value);  // Установка значения по ключу

 private:
  std::map<std::string, std::string> settings;  // Хранение пар "ключ-значение"
  void loadSettings();  // Загрузка настроек из файла
  void saveSettings();  // Сохранение настроек в файл
};

#pragma once
#include <Arduino.h>

#include <map>

class TSettings {
 public:
  TSettings();  // Конструктор для загрузки настроек
  String getValue(const String &key,
                  const String &defaultValue);  // Получение значения по ключу
  void setValue(const String &key,
                const String &value);  // Установка значения по ключу

 private:
  std::map<std::string, std::string> settings;  // Хранение пар "ключ-значение"
  void loadSettings();  // Загрузка настроек из файла
  void saveSettings();  // Сохранение настроек в файл
};

#pragma once
#include <Arduino.h>
#include <LittleFS.h>

#include <map>

// Класс TSettings для работы с настройками
class TSettings {
 public:
  TSettings();  // Конструктор, который считывает настройки из файла при
                // создании объекта
  String getValue(const String &key,
                  const String &defaultValue);  // Получение значения по ключу
  void setValue(const String &key,
                const String &value);  // Установка значения и запись в файл

 private:
  std::map<std::string, std::string> settings;  // Хранение ключ-значение
  void loadSettings();  // Функция для загрузки настроек из файла
  void saveSettings();  // Функция для сохранения настроек в файл
};

// Конструктор: загружаем настройки из файла при создании объекта
TSettings::TSettings() {
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }

  loadSettings();  // Загружаем настройки из файла при создании объекта
}

// Получаем значение по ключу, если ключа нет — возвращаем значение по умолчанию
String TSettings::getValue(const String &key, const String &defaultValue) {
  auto it = settings.find(std::string(
      key.c_str()));  // Преобразуем key в std::string Ищем ключ в словаре
  if (it != settings.end()) {
    return String(
        it->second.c_str());  // Если ключ найден, возвращаем его значение
  }
  return defaultValue;  // Если ключ не найден, возвращаем значение по умолчанию
}

// Устанавливаем значение по ключу и записываем изменения в файл
void TSettings::setValue(const String &key, const String &value) {
  settings[key.c_str()] =
      value.c_str();  // Устанавливаем новое значение в словарь
  saveSettings();  // Сохраняем изменения в файл
}

// Функция для загрузки настроек из файла settings.ini
void TSettings::loadSettings() {
  File file = LittleFS.open("/settings.ini", "r");  // Открываем файл на чтение
  if (!file) {
    Serial.println("Failed to open settings.ini");
    return;
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');  // Читаем файл построчно
    line.trim();  // Убираем лишние пробелы и переносы строк

    if (line.length() == 0 || line[0] == '#')
      continue;  // Пропускаем пустые строки и комментарии

    int delimiterIndex =
        line.indexOf('=');  // Ищем разделитель между ключом и значением
    if (delimiterIndex == -1) continue;  // Пропускаем некорректные строки

    String key = line.substring(0, delimiterIndex);  // Извлекаем ключ
    String value = line.substring(delimiterIndex + 1);  // Извлекаем значение

    settings[key.c_str()] =
        value.c_str();  // Сохраняем ключ и значение в словаре
  }

  file.close();  // Закрываем файл после прочтения
}

// Функция для сохранения настроек в файл settings.ini
void TSettings::saveSettings() {
  File file = LittleFS.open("/settings.ini", "w");  // Открываем файл на запись
  if (!file) {
    Serial.println("Failed to open settings.ini for writing");
    return;
  }

  // Записываем все настройки из словаря в файл
  for (const auto &pair : settings) {
    file.println(
        String(pair.first.c_str()) + "=" +
        String(pair.second.c_str()));  // Записываем строку ключ=значение
  }

  file.close();  // Закрываем файл после записи
}

#include "TSettings.h"

void TSettings::load() {
  String content = sd_card->read(file_name);

  if (!content.isEmpty()) {
    Serial.printf("[SET] Loaded content: %s", content.c_str());
    DeserializationError error = deserializeJson(json_doc, content);
    if (error) {
      Serial.printf("[SET] Deserialization() failed: %s", error.c_str());
    } else {
      if (json_doc.is<JsonArray>()) {
        JsonArray array = json_doc.as<JsonArray>();
        for (JsonVariant value : array) {
          if (value.isNull()) {
            Serial.println(F("[SET] Array contains null value!"));
            json_doc.clear();
          }
        }
      }
    }
  }
}

void TSettings::save() {
  String content;
  serializeJson(json_doc, content);

  Serial.printf("[SET] Saved content: %s\n", content.c_str());

  sd_card->save(file_name, content);
}

String TSettings::getString(const String &key, String default_value) {
  return json_doc[key].is<String>() ? json_doc[key].as<String>()
                                    : default_value;
}
uint32_t TSettings::getInt(const String &key, uint32_t default_value) {
  return json_doc[key].is<uint32_t>() ? json_doc[key].as<uint32_t>()
                                      : default_value;
}
boolean TSettings::getBoolean(const String &key, boolean default_value) {
  return json_doc[key].is<boolean>() ? json_doc[key].as<boolean>()
                                     : default_value;
}

void TSettings::set(const String &key, const String &value) {
  if (!key.isEmpty() && !value.isEmpty()) {
    json_doc[key] = value;
  }
}

void TSettings::set(const String &key, const uint32_t &value) {
  if (!key.isEmpty()) {
    json_doc[key] = value;
  }
}

void TSettings::set(const String &key, const boolean &value) {
  if (!key.isEmpty()) {
    json_doc[key] = value;
  }
}

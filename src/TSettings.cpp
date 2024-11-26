#include "TSettings.h"

void TSettings::load() {
  String content = sd_card->read(file_name);

  if (!content.isEmpty()) {
    Serial.printf("[SET] Loaded content: %s\n", content.c_str());
    DeserializationError error = deserializeJson(json_doc, content);
    if (error) {
      Serial.printf("[SET] Deserialization() failed: %s\n", error.c_str());
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

String TSettings::get(const String &key) { return json_doc[key].as<String>(); }

void TSettings::set(const String &key, const String &value) {
  json_doc[key] = value;
}
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

String mac = WiFi.macAddress();
String ssid = "ESP8266_" + mac.substring(mac.length() - 4);
const char* password = "esp12345";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Подключение к WiFi...");
  }

  Serial.println("Подключено к WiFi");
  Serial.printf("IP адрес: %s", WiFi.localIP().toString().c_str());

  // Обработка корневого URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", "Добро пожаловать на ESP8266!");
  });

  // Запуск сервера
  server.begin();
}

void loop() {
  // Основной цикл может быть пустым, так как мы используем асинхронный сервер
}
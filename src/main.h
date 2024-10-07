#ifndef ESP8266_BASE_V2_BASE_H_
#define ESP8266_BASE_V2_BASE_H_

#include <Arduino.h>
#include <DNSServer.h>
#include <GyverTimer.h>
#include <TSettings.h>

#include "TPin.h"
#include "TTime.h"
#include "TWeb.h"

String getSsid();
void triggerOneSecond();
void initPins();
void listFiles();

#endif  // ESP8266_BASE_V2_BASE_H_
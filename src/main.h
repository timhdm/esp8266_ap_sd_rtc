#pragma once

#include <Arduino.h>
#include <DNSServer.h>
#include <GyverTimer.h>
#include <Preferences.h>

#include <vector>

#include "TLittleFS.h"
#include "TPin.h"
#include "TSdCard.h"
#include "TTime.h"
#include "TWeb.h"

String getSsid();
void triggerOneSecond();
void initPins();

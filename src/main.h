#pragma once

#include <Arduino.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <Wire.h>

#include <vector>

#include "TLittleFS.h"
#include "TPin.h"
#include "TSdCard.h"
#include "TTime.h"
#include "TWeb.h"

String getSsid();
void trigger_one_minute();
void initPins();

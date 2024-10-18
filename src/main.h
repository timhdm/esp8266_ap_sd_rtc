#pragma once

#include <Arduino.h>
#include <DNSServer.h>
#include <GyverTimer.h>
#include <Preferences.h>
#include <Wire.h>

#include <vector>

#include "TDayScheduler.h"
#include "TLittleFS.h"
#include "TPin.h"
#include "TRtc.h"
#include "TSdCard.h"
#include "TTime.h"
#include "TWeb.h"

String getSsid();
void loop_scheduler(const time_t& now_unixtime);
void initPins();

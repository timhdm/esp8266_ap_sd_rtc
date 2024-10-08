#include <Arduino.h>

#include <vector>

#include "TTime.h"

extern TTime timeNow;

class TPinStateLog {
  struct PinState {
    String pin;
    uint8_t state;  // Состояние пина (включен/выключен)
    time_t timestamp;  // Временная метка

    PinState(String pin, uint8_t state)
        : pin(pin), state(state), timestamp(timeNow.getOnlineSeconds()) {}
  };

 public:
  void add(String pin, uint8_t state) { log.push_back(PinState(pin, state)); }
  String getLast100() {
    String result = "";

    size_t count = log.size() < 100 ? log.size() : 100;

    for (size_t i = log.size() - count; i < log.size(); ++i) {
      result += "Pin: " + String(log[i].pin) +
                ", State: " + String(log[i].state ? "ON" : "OFF") +
                ", Time: " + String(log[i].timestamp) + "\n";
    }

    return result;
  }

 private:
  std::vector<PinState> log;
};
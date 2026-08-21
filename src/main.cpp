#include <Arduino.h>
#include <Preferences.h>

#include "app_mode.h"
#include "apps.h"

namespace {
constexpr gpio_num_t kModeButton = GPIO_NUM_1;
Preferences prefs;
AppMode mode = AppMode::Stocks;

void chooseMode() {
  pinMode(kModeButton, INPUT_PULLUP);
  prefs.begin("papercolor", false);
  uint8_t value = prefs.getUChar("mode", 0) % kAppModeCount;
  delay(40);
  if (digitalRead(kModeButton) == LOW) {
    value = (value + 1) % kAppModeCount;
    prefs.putUChar("mode", value);
    while (digitalRead(kModeButton) == LOW) delay(10);
  }
  prefs.end();
  mode = static_cast<AppMode>(value);
}
}

void setup() {
  Serial.begin(115200);
  chooseMode();
  switch (mode) {
    case AppMode::Stocks: stockSetup(); break;
    case AppMode::Reader: readerSetup(); break;
    case AppMode::Calendar: calendarSetup(); break;
    case AppMode::PhotoFrame: photoFrameSetup(); break;
    case AppMode::Sudoku: sudokuSetup(); break;
  }
}

void loop() {
  switch (mode) {
    case AppMode::Stocks: stockLoop(); break;
    case AppMode::Reader: readerLoop(); break;
    case AppMode::Calendar: calendarLoop(); break;
    case AppMode::PhotoFrame: photoFrameLoop(); break;
    case AppMode::Sudoku: sudokuLoop(); break;
  }
}

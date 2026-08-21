#include "native_display.h"

namespace {
constexpr int kSdSclk = 15;
constexpr int kSdMiso = 14;
constexpr int kSdMosi = 13;
constexpr int kSdCs = 47;
bool sdReady = false;
}

bool beginNativeDisplay() {
  auto cfg = M5.config();
  cfg.clear_display = false;
  M5.begin(cfg);
  M5.Display.setRotation(1);
  M5.Display.setEpdMode(epd_mode_t::epd_quality);
  M5.Display.fillScreen(WHITE);
  M5.Display.setTextColor(BLACK, WHITE);
  return true;
}

bool beginSharedSd() {
  if (sdReady) return true;
  SPI.begin(kSdSclk, kSdMiso, kSdMosi, kSdCs);
  sdReady = SD.begin(kSdCs, SPI, 20000000);
  return sdReady;
}

void nativeHeader(const char* title, uint32_t accent) {
  M5.Display.fillRect(0, 0, 600, 54, accent);
  M5.Display.setTextColor(WHITE, accent);
  M5.Display.setTextDatum(middle_left);
  M5.Display.setFont(&fonts::FreeSansBold18pt7b);
  M5.Display.drawString(title, 18, 28);
  M5.Display.setTextColor(BLACK, WHITE);
}

void nativeFooter(const char* text) {
  M5.Display.drawFastHLine(16, 370, 568, BLACK);
  M5.Display.setFont(&fonts::FreeSans9pt7b);
  M5.Display.setTextDatum(middle_center);
  M5.Display.drawString(text, 300, 384);
}


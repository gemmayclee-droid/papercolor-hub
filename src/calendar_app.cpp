#include "apps.h"

#include <M5Unified.h>
#include <WiFi.h>
#include <ctime>

#include "config.h"
#include "i18n.h"
#include "native_display.h"

namespace {
int monthOffset = 0;
uint32_t nextClockRefresh = 0;
Locale locale = Locale::ZhTW;

bool leap(int year) { return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0); }

int daysInMonth(int year, int month) {
  static constexpr uint8_t days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  return month == 2 && leap(year) ? 29 : days[month - 1];
}

int weekday(int year, int month, int day) {
  if (month < 3) { month += 12; --year; }
  const int k = year % 100;
  const int j = year / 100;
  return (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j + 6) % 7;
}

void shiftedMonth(int& year, int& month, int& today) {
  time_t now = time(nullptr);
  tm local{};
  localtime_r(&now, &local);
  year = local.tm_year + 1900;
  month = local.tm_mon + 1 + monthOffset;
  while (month > 12) { month -= 12; ++year; }
  while (month < 1) { month += 12; --year; }
  today = monthOffset == 0 ? local.tm_mday : -1;
}

void syncClock() {
  DropboxGalleryConfig config;
  if (!beginSharedSd() || !loadDropboxGalleryConfig(config)) return;
  WiFi.mode(WIFI_STA);
  WiFi.begin(config.wifiSsid.c_str(), config.wifiPassword.c_str());
  const uint32_t wifiStarted = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - wifiStarted < 20000) delay(200);
  if (WiFi.status() == WL_CONNECTED) {
    configTzTime(config.timezone.c_str(), "pool.ntp.org", "time.nist.gov");
    const uint32_t timeStarted = millis();
    while (time(nullptr) < 1700000000 && millis() - timeStarted < 10000) delay(200);
  }
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

const char* weekdayName(int value) {
  static constexpr const char* kWeekdays[][3] = {
      {"週日", "周日", "Sun"}, {"週一", "周一", "Mon"}, {"週二", "周二", "Tue"},
      {"週三", "周三", "Wed"}, {"週四", "周四", "Thu"}, {"週五", "周五", "Fri"},
      {"週六", "周六", "Sat"},
  };
  return kWeekdays[value][static_cast<uint8_t>(locale)];
}

void renderCalendar() {
  int year, month, today;
  shiftedMonth(year, month, today);
  M5.Display.startWrite();
  M5.Display.fillScreen(WHITE);
  time_t now = time(nullptr);
  tm local{};
  localtime_r(&now, &local);
  char title[64];
  snprintf(title, sizeof(title), "%04d-%02d-%02d %s  %02d:%02d", local.tm_year + 1900,
           local.tm_mon + 1, local.tm_mday, weekdayName(local.tm_wday), local.tm_hour, local.tm_min);
  nativeHeader(title, BLUE);
  const int cellW = 82;
  const int x0 = 13;
  M5.Display.setFont(&fonts::FreeSansBold9pt7b);
  M5.Display.setTextDatum(middle_center);
  for (int col = 0; col < 7; ++col) {
    M5.Display.setTextColor(col == 0 ? RED : (col == 6 ? BLUE : BLACK), WHITE);
    M5.Display.drawString(weekdayName(col), x0 + col * cellW + cellW / 2, 78);
  }
  const int first = weekday(year, month, 1);
  const int count = daysInMonth(year, month);
  M5.Display.setFont(&fonts::FreeSansBold12pt7b);
  for (int day = 1; day <= count; ++day) {
    const int index = first + day - 1;
    const int col = index % 7;
    const int row = index / 7;
    const int cx = x0 + col * cellW + cellW / 2;
    const int cy = 116 + row * 43;
    if (day == today) M5.Display.fillCircle(cx, cy, 18, YELLOW);
    M5.Display.setTextColor(col == 0 ? RED : (col == 6 ? BLUE : BLACK), day == today ? YELLOW : WHITE);
    M5.Display.drawNumber(day, cx, cy);
  }
  nativeFooter(tr(TextId::CalendarFooter, locale));
  M5.Display.endWrite();
  nextClockRefresh = millis() + 60UL * 1000UL;
}
}

void calendarSetup() {
  beginNativeDisplay();
  locale = detectLocale();
  syncClock();
  renderCalendar();
}

void calendarLoop() {
  M5.update();
  if (M5.BtnA.wasPressed()) { --monthOffset; renderCalendar(); }
  if (M5.BtnB.wasPressed()) { monthOffset = 0; renderCalendar(); }
  if (M5.BtnC.wasPressed()) { ++monthOffset; renderCalendar(); }
  if (static_cast<int32_t>(millis() - nextClockRefresh) >= 0) renderCalendar();
  delay(20);
}

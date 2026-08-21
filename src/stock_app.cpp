#include "apps.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <M5Unified.h>
#include <WiFi.h>
#include <vector>

#include "config.h"
#include "native_display.h"

namespace {
struct Quote {
  String symbol;
  String name;
  float price = 0;
  float previous = 0;
  bool valid = false;
};

StockConfig config;
std::vector<Quote> quotes;
uint32_t nextRefresh = 0;

String channelFor(const String& symbol) {
  if (symbol.startsWith("otc_")) return symbol + ".tw";
  if (symbol.startsWith("tse_")) return symbol + ".tw";
  return "tse_" + symbol + ".tw";
}

float firstNumber(const char* value) {
  if (!value) return 0;
  String text(value);
  const int underscore = text.indexOf('_');
  if (underscore >= 0) text = text.substring(0, underscore);
  return text == "-" ? 0 : text.toFloat();
}

bool fetchQuotes() {
  if (WiFi.status() != WL_CONNECTED) return false;
  String channels;
  for (size_t i = 0; i < config.symbols.size(); ++i) {
    if (i) channels += '|';
    channels += channelFor(config.symbols[i]);
  }
  HTTPClient http;
  const String url = "https://mis.twse.com.tw/stock/api/getStockInfo.jsp?ex_ch=" + channels + "&json=1&delay=0";
  http.setTimeout(10000);
  if (!http.begin(url)) return false;
  http.addHeader("User-Agent", "PaperColorHub/0.1");
  const int status = http.GET();
  if (status != HTTP_CODE_OK) {
    http.end();
    return false;
  }
  JsonDocument doc;
  const auto error = deserializeJson(doc, http.getStream());
  http.end();
  if (error) return false;
  quotes.clear();
  for (JsonObject item : doc["msgArray"].as<JsonArray>()) {
    Quote quote;
    quote.symbol = item["c"] | "";
    quote.name = item["n"] | quote.symbol;
    quote.price = firstNumber(item["z"]);
    if (quote.price <= 0) quote.price = firstNumber(item["b"]);
    quote.previous = firstNumber(item["y"]);
    quote.valid = quote.price > 0;
    quotes.push_back(quote);
  }
  return !quotes.empty();
}

void render() {
  M5.Display.startWrite();
  M5.Display.fillScreen(WHITE);
  nativeHeader("TW STOCK WATCHLIST", RED);
  M5.Display.setTextDatum(middle_left);
  M5.Display.setFont(&fonts::FreeSansBold12pt7b);
  int y = 85;
  for (const auto& quote : quotes) {
    if (y > 340) break;
    const float change = quote.price - quote.previous;
    const float pct = quote.previous > 0 ? change * 100.0f / quote.previous : 0;
    const uint32_t color = change > 0 ? RED : (change < 0 ? GREEN : BLACK);
    M5.Display.setTextColor(BLACK, WHITE);
    M5.Display.drawString(quote.symbol, 24, y);
    M5.Display.setTextColor(color, WHITE);
    M5.Display.setTextDatum(middle_right);
    M5.Display.drawFloat(quote.price, 2, 360, y);
    char delta[40];
    snprintf(delta, sizeof(delta), "%+.2f  %+.2f%%", change, pct);
    M5.Display.drawString(delta, 575, y);
    M5.Display.drawFastHLine(20, y + 25, 560, LIGHTGREY);
    M5.Display.setTextDatum(middle_left);
    y += 52;
  }
  if (quotes.empty()) {
    M5.Display.setTextDatum(middle_center);
    M5.Display.drawString("No quote data", 300, 210);
  }
  nativeFooter("Hold button C while booting to switch mode");
  M5.Display.endWrite();
}
}

void stockSetup() {
  beginNativeDisplay();
  if (!beginSharedSd() || !loadStockConfig(config)) {
    quotes.clear();
    render();
    return;
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(config.wifiSsid.c_str(), config.wifiPassword.c_str());
  const uint32_t started = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - started < 12000) delay(100);
  fetchQuotes();
  render();
  nextRefresh = millis() + config.refreshSeconds * 1000UL;
}

void stockLoop() {
  M5.update();
  if (M5.BtnB.wasPressed() || (nextRefresh && static_cast<int32_t>(millis() - nextRefresh) >= 0)) {
    fetchQuotes();
    render();
    nextRefresh = millis() + config.refreshSeconds * 1000UL;
  }
  delay(20);
}

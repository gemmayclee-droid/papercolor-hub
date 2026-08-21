#include "config.h"

#include <ArduinoJson.h>
#include <SD.h>

bool loadStockConfig(StockConfig& config) {
  File file = SD.open("/config/stocks.json", FILE_READ);
  if (!file) return false;
  JsonDocument doc;
  const auto error = deserializeJson(doc, file);
  file.close();
  if (error) return false;
  config.wifiSsid = doc["wifi"]["ssid"] | "";
  config.wifiPassword = doc["wifi"]["password"] | "";
  config.refreshSeconds = doc["refresh_seconds"] | 300;
  for (JsonVariant symbol : doc["symbols"].as<JsonArray>()) {
    String value = symbol.as<String>();
    value.trim();
    if (value.length() > 0) config.symbols.push_back(value);
  }
  return config.wifiSsid.length() > 0 && !config.symbols.empty();
}


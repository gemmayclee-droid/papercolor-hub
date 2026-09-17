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

bool loadDropboxGalleryConfig(DropboxGalleryConfig& config) {
  File file = SD.open("/config/dropbox.json", FILE_READ);
  if (!file) return false;
  JsonDocument doc;
  const auto error = deserializeJson(doc, file);
  file.close();
  if (error) return false;

  config.wifiSsid = doc["wifi"]["ssid"] | "";
  config.wifiPassword = doc["wifi"]["password"] | "";
  config.appKey = doc["dropbox"]["app_key"] | "";
  config.refreshToken = doc["dropbox"]["refresh_token"] | "";
  config.folder = doc["dropbox"]["folder"] | "/Apps/PaperColor Gallery";
  config.refreshSeconds = doc["refresh_seconds"] | (12UL * 60UL * 60UL);
  if (config.refreshSeconds < 60) config.refreshSeconds = 60;
  return config.wifiSsid.length() > 0 && config.appKey.length() > 0 &&
         config.refreshToken.length() > 0;
}

#include "config.h"

#include <ArduinoJson.h>
#include <SD.h>

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
  config.timezone = doc["timezone"] | "CST-8";
  config.refreshSeconds = doc["refresh_seconds"] | (12UL * 60UL * 60UL);
  if (config.refreshSeconds < 60) config.refreshSeconds = 60;
  return config.wifiSsid.length() > 0 && config.appKey.length() > 0 &&
         config.refreshToken.length() > 0;
}

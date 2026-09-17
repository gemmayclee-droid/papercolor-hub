#pragma once

#include <Arduino.h>
#include <vector>

struct StockConfig {
  String wifiSsid;
  String wifiPassword;
  std::vector<String> symbols;
  uint32_t refreshSeconds = 300;
};

bool loadStockConfig(StockConfig& config);

struct DropboxGalleryConfig {
  String wifiSsid;
  String wifiPassword;
  String appKey;
  String refreshToken;
  String folder = "/Apps/PaperColor Gallery";
  uint32_t refreshSeconds = 12UL * 60UL * 60UL;
};

bool loadDropboxGalleryConfig(DropboxGalleryConfig& config);

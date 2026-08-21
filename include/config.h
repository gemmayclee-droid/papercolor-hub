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


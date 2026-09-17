#pragma once

#include <Arduino.h>
struct DropboxGalleryConfig {
  String wifiSsid;
  String wifiPassword;
  String appKey;
  String refreshToken;
  String folder = "/Apps/PaperColor Gallery";
  String timezone = "CST-8";
  uint32_t refreshSeconds = 12UL * 60UL * 60UL;
};

bool loadDropboxGalleryConfig(DropboxGalleryConfig& config);

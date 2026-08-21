#include "apps.h"

#include <M5Unified.h>
#include <SD.h>
#include <vector>

#include "native_display.h"

namespace {
std::vector<String> photos;
size_t currentPhoto = 0;
uint32_t nextPhoto = 0;
constexpr uint32_t kSlideMs = 5UL * 60UL * 1000UL;

bool isPhoto(const String& name) {
  String lower = name;
  lower.toLowerCase();
  return lower.endsWith(".jpg") || lower.endsWith(".jpeg") || lower.endsWith(".png") || lower.endsWith(".bmp");
}

void scanPhotos() {
  photos.clear();
  File dir = SD.open("/photos");
  if (!dir || !dir.isDirectory()) return;
  File item;
  while ((item = dir.openNextFile())) {
    if (!item.isDirectory()) {
      String name = item.name();
      if (isPhoto(name)) {
        if (!name.startsWith("/")) name = "/photos/" + name;
        photos.push_back(name);
      }
    }
    item.close();
  }
  dir.close();
}

void renderPhoto() {
  M5.Display.startWrite();
  M5.Display.fillScreen(WHITE);
  if (photos.empty()) {
    nativeHeader("PHOTO FRAME", GREEN);
    M5.Display.setFont(&fonts::FreeSansBold18pt7b);
    M5.Display.setTextDatum(middle_center);
    M5.Display.setTextColor(BLACK, WHITE);
    M5.Display.drawString("Put JPG / PNG / BMP in /photos", 300, 210);
  } else {
    const String& path = photos[currentPhoto % photos.size()];
    String lower = path;
    lower.toLowerCase();
    if (lower.endsWith(".jpg") || lower.endsWith(".jpeg")) {
      M5.Display.drawJpgFile(SD, path.c_str(), 0, 0, 600, 400, 0, 0, 1.0f, 1.0f, middle_center);
    } else if (lower.endsWith(".png")) {
      M5.Display.drawPngFile(SD, path.c_str(), 0, 0, 600, 400, 0, 0, 1.0f, 1.0f, middle_center);
    } else {
      M5.Display.drawBmpFile(SD, path.c_str(), 0, 0, 600, 400, 0, 0, 1.0f, 1.0f, middle_center);
    }
  }
  M5.Display.endWrite();
  nextPhoto = millis() + kSlideMs;
}

void movePhoto(int delta) {
  if (photos.empty()) return;
  currentPhoto = (currentPhoto + photos.size() + delta) % photos.size();
  renderPhoto();
}
}

void photoFrameSetup() {
  beginNativeDisplay();
  beginSharedSd();
  scanPhotos();
  renderPhoto();
}

void photoFrameLoop() {
  M5.update();
  if (M5.BtnA.wasPressed()) movePhoto(-1);
  if (M5.BtnB.wasPressed()) { scanPhotos(); renderPhoto(); }
  if (M5.BtnC.wasPressed() || static_cast<int32_t>(millis() - nextPhoto) >= 0) movePhoto(1);
  delay(20);
}

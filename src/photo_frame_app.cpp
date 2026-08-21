#include "apps.h"

#include <M5Unified.h>
#include <SD.h>
#include <esp_heap_caps.h>
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

bool drawPhotoBuffer(const String& path, const String& lower) {
  File file = SD.open(path, FILE_READ);
  if (!file || file.isDirectory()) return false;
  const size_t size = file.size();
  if (size == 0 || size > 6 * 1024 * 1024) {
    file.close();
    return false;
  }
  auto* data = static_cast<uint8_t*>(
      heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
  if (!data) {
    file.close();
    return false;
  }
  const bool complete = file.read(data, size) == size;
  file.close();
  bool drawn = false;
  if (complete && (lower.endsWith(".jpg") || lower.endsWith(".jpeg"))) {
    drawn = M5.Display.drawJpg(data, size, 0, 0, 600, 400, 0, 0, 1.0f, 1.0f, middle_center);
  } else if (complete && lower.endsWith(".png")) {
    drawn = M5.Display.drawPng(data, size, 0, 0, 600, 400, 0, 0, 1.0f, 1.0f, middle_center);
  } else if (complete) {
    drawn = M5.Display.drawBmp(data, size, 0, 0, 600, 400, 0, 0, 1.0f, 1.0f, middle_center);
  }
  heap_caps_free(data);
  return drawn;
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
    if (!drawPhotoBuffer(path, lower)) {
      nativeHeader("PHOTO FRAME", GREEN);
      M5.Display.setFont(&fonts::FreeSansBold18pt7b);
      M5.Display.setTextDatum(middle_center);
      M5.Display.drawString("Image cannot be decoded", 300, 210);
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

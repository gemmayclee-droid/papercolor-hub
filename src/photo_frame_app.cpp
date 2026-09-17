#include "apps.h"

#include <M5Unified.h>
#include <SD.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <esp_heap_caps.h>
#include <esp_sleep.h>
#include <algorithm>
#include <vector>

#include "config.h"
#include "native_display.h"

namespace {
std::vector<String> photos;
size_t currentPhoto = 0;
uint32_t nextPhoto = 0;
constexpr uint32_t kSlideMs = 5UL * 60UL * 1000UL;
constexpr uint32_t kWifiTimeoutMs = 20UL * 1000UL;

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

bool connectWifi(const DropboxGalleryConfig& config) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(config.wifiSsid.c_str(), config.wifiPassword.c_str());
  const uint32_t started = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - started < kWifiTimeoutMs) {
    delay(200);
  }
  return WiFi.status() == WL_CONNECTED;
}

String urlEncode(const String& value) {
  const char* hex = "0123456789ABCDEF";
  String result;
  for (size_t i = 0; i < value.length(); ++i) {
    const uint8_t c = static_cast<uint8_t>(value[i]);
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~') {
      result += static_cast<char>(c);
    } else {
      result += '%';
      result += hex[c >> 4];
      result += hex[c & 0x0f];
    }
  }
  return result;
}

bool postDropboxJson(const char* endpoint, const String& accessToken, const String& request,
                     String& response) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  if (!http.begin(client, String("https://api.dropboxapi.com/2/") + endpoint)) return false;
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + accessToken);
  const int status = http.POST(request);
  response = status == HTTP_CODE_OK ? http.getString() : "";
  http.end();
  return status == HTTP_CODE_OK;
}

bool getDropboxAccessToken(const DropboxGalleryConfig& config, String& accessToken) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  if (!http.begin(client, "https://api.dropboxapi.com/oauth2/token")) return false;
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  const String request = "grant_type=refresh_token&refresh_token=" +
                         urlEncode(config.refreshToken) + "&client_id=" +
                         urlEncode(config.appKey);
  const int status = http.POST(request);
  if (status != HTTP_CODE_OK) {
    http.end();
    return false;
  }
  JsonDocument doc;
  const auto error = deserializeJson(doc, http.getString());
  http.end();
  if (error) return false;
  accessToken = doc["access_token"] | "";
  return accessToken.length() > 0;
}

void addPhotosFromResponse(const String& response, std::vector<String>& paths, bool& hasMore,
                           String& cursor) {
  hasMore = false;
  cursor = "";
  JsonDocument doc;
  if (deserializeJson(doc, response)) return;
  hasMore = doc["has_more"] | false;
  cursor = doc["cursor"] | "";
  for (JsonVariant entry : doc["entries"].as<JsonArray>()) {
    if (String(entry[".tag"] | "") != "file") continue;
    String path = entry["path_display"] | "";
    if (isPhoto(path)) paths.push_back(path);
  }
}

bool listDropboxPhotos(const DropboxGalleryConfig& config, const String& accessToken,
                       std::vector<String>& paths) {
  JsonDocument request;
  request["path"] = config.folder;
  request["recursive"] = true;
  request["include_deleted"] = false;
  String body;
  serializeJson(request, body);
  String response;
  if (!postDropboxJson("files/list_folder", accessToken, body, response)) return false;

  bool hasMore = false;
  String cursor;
  addPhotosFromResponse(response, paths, hasMore, cursor);
  while (hasMore && cursor.length() > 0) {
    JsonDocument continuation;
    continuation["cursor"] = cursor;
    serializeJson(continuation, body);
    if (!postDropboxJson("files/list_folder/continue", accessToken, body, response)) return false;
    addPhotosFromResponse(response, paths, hasMore, cursor);
  }
  std::sort(paths.begin(), paths.end());
  return !paths.empty();
}

bool downloadDropboxPhoto(const String& accessToken, const String& sourcePath, String& cachePath) {
  cachePath = "/photos/dropbox-current.jpg";
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  if (!http.begin(client, "https://content.dropboxapi.com/2/files/get_thumbnail_v2")) return false;
  JsonDocument argument;
  JsonObject resource = argument["resource"].to<JsonObject>();
  resource[".tag"] = "path";
  resource["path"] = sourcePath;
  argument["format"] = "jpeg";
  argument["size"] = "w1024h768";
  argument["mode"] = "bestfit";
  String argumentJson;
  serializeJson(argument, argumentJson);
  http.addHeader("Authorization", "Bearer " + accessToken);
  http.addHeader("Dropbox-API-Arg", argumentJson);
  const int status = http.POST("");
  if (status != HTTP_CODE_OK) {
    http.end();
    return false;
  }
  if (SD.exists(cachePath)) SD.remove(cachePath);
  File output = SD.open(cachePath, FILE_WRITE);
  if (!output) {
    http.end();
    return false;
  }
  WiFiClient* stream = http.getStreamPtr();
  uint8_t buffer[1024];
  int remaining = http.getSize();
  while (http.connected() && (remaining > 0 || remaining == -1)) {
    const size_t available = stream->available();
    if (!available) {
      delay(1);
      continue;
    }
    const size_t count = stream->readBytes(buffer, std::min(available, sizeof(buffer)));
    if (!count || output.write(buffer, count) != count) {
      output.close();
      http.end();
      return false;
    }
    if (remaining > 0) remaining -= count;
  }
  output.close();
  http.end();
  return SD.exists(cachePath);
}

String nextDropboxPath(const std::vector<String>& paths) {
  Preferences prefs;
  prefs.begin("dropbox", true);
  const String previous = prefs.getString("last_path", "");
  prefs.end();
  size_t index = 0;
  const auto found = std::find(paths.begin(), paths.end(), previous);
  if (found != paths.end()) index = (static_cast<size_t>(found - paths.begin()) + 1) % paths.size();
  return paths[index];
}

void rememberDropboxPath(const String& path) {
  Preferences prefs;
  prefs.begin("dropbox", false);
  prefs.putString("last_path", path);
  prefs.end();
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

void renderDropboxPhoto(const String& path) {
  M5.Display.startWrite();
  M5.Display.fillScreen(WHITE);
  String lower = path;
  lower.toLowerCase();
  if (!drawPhotoBuffer(path, lower)) {
    nativeHeader("DROPBOX GALLERY", BLUE);
    M5.Display.setFont(&fonts::FreeSansBold18pt7b);
    M5.Display.setTextDatum(middle_center);
    M5.Display.drawString("Image cannot be decoded", 300, 210);
  }
  M5.Display.endWrite();
}

void sleepUntilDropboxRefresh(uint32_t seconds) {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  M5.Display.waitDisplay();
  esp_sleep_enable_timer_wakeup(static_cast<uint64_t>(seconds) * 1000000ULL);
  esp_deep_sleep_start();
}

void runDropboxGallery(const DropboxGalleryConfig& config) {
  if (!connectWifi(config)) {
    scanPhotos();
    renderPhoto();
    sleepUntilDropboxRefresh(config.refreshSeconds);
  }
  String accessToken;
  std::vector<String> remotePhotos;
  String cachePath;
  if (getDropboxAccessToken(config, accessToken) &&
      listDropboxPhotos(config, accessToken, remotePhotos)) {
    const String selected = nextDropboxPath(remotePhotos);
    if (downloadDropboxPhoto(accessToken, selected, cachePath)) {
      rememberDropboxPath(selected);
      renderDropboxPhoto(cachePath);
    } else {
      cachePath = "";
    }
  }
  if (cachePath.isEmpty()) {
    scanPhotos();
    renderPhoto();
  }
  sleepUntilDropboxRefresh(config.refreshSeconds);
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
  DropboxGalleryConfig dropbox;
  if (loadDropboxGalleryConfig(dropbox)) {
    runDropboxGallery(dropbox);
    return;
  }
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

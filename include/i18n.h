#pragma once

#include <cstdint>

enum class Locale : uint8_t { ZhTW, ZhCN, En };

enum class TextId : uint8_t {
  Calendar,
  DropboxGallery,
  NoPhotos,
  ImageDecodeError,
  LocalPhotosHint,
  CalendarFooter,
};

Locale detectLocale();
const char* tr(TextId id, Locale locale);

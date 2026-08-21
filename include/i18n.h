#pragma once

#include <cstdint>

enum class Locale : uint8_t { ZhTW, ZhCN, En };

enum class TextId : uint8_t {
  Stocks,
  Reader,
  Calendar,
  PhotoFrame,
  Sudoku,
  Updated,
  Change,
  NoData,
  NoPhotos,
  Today,
  Connecting,
  ConfigError,
  Solved,
};

Locale detectLocale();
const char* tr(TextId id, Locale locale);

#pragma once

#include <cstdint>

enum class AppMode : uint8_t {
  Stocks = 0,
  Reader = 1,
  Calendar = 2,
  PhotoFrame = 3,
};

constexpr uint8_t kAppModeCount = 4;


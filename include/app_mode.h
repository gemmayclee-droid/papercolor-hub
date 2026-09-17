#pragma once

#include <cstdint>

enum class AppMode : uint8_t {
  DropboxGallery = 0,
  Calendar = 1,
};

constexpr uint8_t kAppModeCount = 2;

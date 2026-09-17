#include "i18n.h"

#include <Arduino.h>

namespace {
constexpr const char* kTexts[][3] = {
    {"萬年曆", "万年历", "Calendar"},
    {"Dropbox 藝廊", "Dropbox 画廊", "Dropbox Gallery"},
    {"找不到照片", "找不到照片", "No photos"},
    {"無法解碼圖片", "无法解码图片", "Image cannot be decoded"},
    {"將 JPG、PNG 或 BMP 放入 /photos", "将 JPG、PNG 或 BMP 放入 /photos", "Put JPG / PNG / BMP in /photos"},
    {"A 上月　B 本月　C 下月", "A 上月　B 本月　C 下月", "A previous month   B current month   C next month"},
};
}

Locale detectLocale() {
#if defined(PAPERCOLOR_LOCALE_ZH_CN)
  return Locale::ZhCN;
#elif defined(PAPERCOLOR_LOCALE_EN)
  return Locale::En;
#else
  return Locale::ZhTW;
#endif
}

const char* tr(TextId id, Locale locale) {
  return kTexts[static_cast<uint8_t>(id)][static_cast<uint8_t>(locale)];
}

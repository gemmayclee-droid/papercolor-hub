#include "i18n.h"

#include <Arduino.h>

namespace {
constexpr const char* kTexts[][3] = {
    {"台股自選", "台股自选", "TW Stocks"},
    {"閱讀器", "阅读器", "Reader"},
    {"萬年曆", "万年历", "Calendar"},
    {"彩色相框", "彩色相框", "Photo Frame"},
    {"數獨", "数独", "Sudoku"},
    {"更新", "更新", "Updated"},
    {"漲跌", "涨跌", "Change"},
    {"暫無資料", "暂无数据", "No data"},
    {"找不到照片", "找不到照片", "No photos"},
    {"今天", "今天", "Today"},
    {"正在連線", "正在连接", "Connecting"},
    {"設定錯誤", "设置错误", "Configuration error"},
    {"完成！", "完成！", "Solved!"},
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

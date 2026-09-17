# 项目历史

本文从先前的 Codex 任务“M5Stack PaperColor 固件”迁入，保留对当前项目仍有价值的决策、功能与发布记录；不包含逐字对话、工具输出或账号信息。

## 起源与技术决策

- 创建 `gemmayclee-droid/papercolor-hub`，作为 M5Stack PaperColor C151 的整合式固件。
- 显示与硬件采用官方 `M5Unified`、`M5GFX` 和 `M5PM1`。
- 阅读器采用 FreeInkBook／FreeInk 技术路线，支持 EPUB ZIP、CSS、CJK 排版、外部 TTF/OTF 字体、图片、书签和缓存。
- 台股行情采用台湾证券交易所 MIS 公开端点。
- 相框只借鉴 GPL-3.0 项目的功能方向，主项目自行实现，以保持 MIT 许可兼容性。

## 功能里程碑

1. 完成四个初始模式：台湾自选股、EPUB/TXT 阅读器、桌面万年历和彩色相框。
2. 修正相框的 SD 文件接口兼容问题：图像先读入 PSRAM，再交由 M5GFX 的内存解码 API 显示，保留 JPG、PNG 和 BMP 支持。
3. 新增第五个模式“数独”：A/C 移动、B 填数、长按 B 清除、长按 A 重设；冲突标记与进度保存于 NVS。
4. 所有界面与使用说明同步提供繁体中文、简体中文和英文。

## 构建与发布

- 不要求用户在本机安装 PlatformIO；GitHub Actions 负责递归取得 FreeInk 子模块、编译 C151 固件并收集 `.bin`。
- Git tag 符合 `v*` 时，工作流程会创建 GitHub Release，附上 `firmware.bin`、`bootloader.bin` 和 `partitions.bin`。
- `v0.1.0` 已建立为首个正式 Release；历史任务记录中在线编译成功，但实体设备按键手感与画面刷新仍应以每次硬件测试为准。

## 相关资源调查

先前也整理过 PaperColor 生态：官方 UserDemo、M5Unified/M5GFX/M5PM1、FreeInk SDK、ESPHome 集成、万年历、相框及信息面板等。实现时应优先确认各上游项目的最新状态与许可。

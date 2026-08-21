# PaperColor Hub

[繁體中文](../README.md) · [简体中文](README.zh-CN.md) · [English](README.en.md)

这是用于 M5Stack PaperColor（C151）的五合一开源固件：台湾股票自选行情、
EPUB/TXT 阅读器、桌面万年历、彩色电子纸相框和数独游戏。

数独模式使用 A／C 移动，B 填写数字；长按 B 清除，长按 A 重置题目。
冲突数字以红色显示，游戏进度自动保存。

关机后按住 C 键再开机可切换模式。将 `examples/sd-card` 的目录复制到 microSD：

- `/config/stocks.json`：Wi-Fi、刷新间隔和股票代码；上柜股票使用 `otc_` 前缀。
- `/Books/*.epub`：无 DRM 的 EPUB/TXT 图书。
- `/fonts/*.ttf`：阅读字体，可加入 Bold、Italic 和 BoldItalic 字体。
- `/photos/`：JPG、PNG 或 BMP 照片。

使用 PlatformIO 执行 `pio run -e m5papercolor` 编译。首次克隆仓库时请使用
`git clone --recurse-submodules`。

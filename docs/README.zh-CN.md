# PaperColor Hub

[繁體中文](../README.md) · [简体中文](README.zh-CN.md) · [English](README.en.md)

这是用于 M5Stack PaperColor（C151）的双模式开源固件：Dropbox 循环画廊，以及显示今天日期、星期和时间的桌面万年历。

关机后按住 C 键再开机可在两个模式之间切换，选择会保存到 NVS。画廊会递归读取已配置的 Dropbox 文件夹、跳过视频、每 12 小时显示下一张照片后进入深度睡眠。请参阅 [Dropbox 画廊设置](DROPBOX.zh-CN.md)。

万年历操作：A 上个月、B 回到本月、C 下个月。它使用 `/config/dropbox.json` 内的 Wi-Fi 和 `timezone` 设置，通过 NTP 校正时间。

请将以下必要配置复制到 microSD：

- `/config/dropbox.json`：Dropbox 凭证、Wi-Fi、时区、画廊路径和刷新间隔。
- `/photos/`：Dropbox 无法使用时的可选 JPG、PNG 或 BMP 备用照片。

无需在电脑上安装 PlatformIO。请从 [Releases](../../releases) 下载最新版本的三个
`.bin` 文件，并按照[安装说明](INSTALL.zh-CN.md)烧录。

如需自行编译，请 fork 本仓库后，在 GitHub **Actions** 中运行
**Build PaperColor firmware**。完成后可从 Artifact 下载；推送 `v*` 格式的 tag
时，GitHub Actions 会自动创建带 `.bin` 文件的 Release。

# 安装 PaperColor Hub

[繁體中文](INSTALL.zh-TW.md) · [简体中文](INSTALL.zh-CN.md) · [English](INSTALL.en.md)

本说明适用于 M5Stack PaperColor C151。烧录前请备份设备中的重要资料；以下命令会清除
原有固件和设置。

## 1. 下载 Release

从 [Releases](../../releases) 下载同一版本的三个文件并解压：

- `papercolor-hub-bootloader.bin`
- `papercolor-hub-partitions.bin`
- `papercolor-hub-firmware.bin`

## 2. 准备烧录工具

仅需 Espressif 的 `esptool`，不需要 PlatformIO。电脑须已安装 Python 3：

```bash
python -m pip install --user esptool
```

使用 USB-C 数据线连接 PaperColor，不能使用仅充电线。确认串口：Windows 通常是 `COM3`，
macOS 通常是 `/dev/cu.usbmodem*`，Linux 通常是 `/dev/ttyACM0` 或 `/dev/ttyUSB0`。

## 3. 擦除并烧录

将下列 `PORT` 改为实际串口，在三个 `.bin` 所在目录执行：

```bash
esptool --chip esp32s3 --port PORT erase-flash
esptool --chip esp32s3 --port PORT --baud 460800 write-flash \
  --flash-mode qio --flash-freq 80m --flash-size 16MB \
  0x0 papercolor-hub-bootloader.bin \
  0x8000 papercolor-hub-partitions.bin \
  0x10000 papercolor-hub-firmware.bin
```

若无法连接，请重新插拔、换用数据线，并根据设备／esptool 的 Bootloader 模式说明重试。

## 4. 首次启动

烧录完成后重启。首次彩色屏幕完整刷新约需 15 秒。将 `examples/sd-card` 内容复制到
microSD 根目录，即可使用股票、阅读器和相框。

更多 ESP32-S3 烧录参数请参考 [Espressif esptool 文档](https://docs.espressif.com/projects/esptool/en/latest/esp32s3/esptool/)。

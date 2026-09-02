# 安裝 PaperColor Hub

[繁體中文](INSTALL.zh-TW.md) · [简体中文](INSTALL.zh-CN.md) · [English](INSTALL.en.md)

本說明適用於 M5Stack PaperColor C151。燒錄前請先備份裝置中任何重要資料；下列
指令會清除原有韌體與設定。

## 1. 下載 Release

從 [Releases](../../releases) 下載同一版本的三個檔案並解壓縮：

- `papercolor-hub-bootloader.bin`
- `papercolor-hub-partitions.bin`
- `papercolor-hub-firmware.bin`

## 2. 準備燒錄工具

此步驟只需要 Espressif 的 `esptool`，不需要 PlatformIO。電腦必須已有 Python 3：

```bash
python -m pip install --user esptool
```

以 USB-C 資料線連接 PaperColor。請勿使用僅供充電的線材。確認序列埠名稱：Windows
通常是 `COM3`，macOS 通常是 `/dev/cu.usbmodem*`，Linux 通常是 `/dev/ttyACM0` 或
`/dev/ttyUSB0`。

## 3. 清除並燒錄

將下列 `PORT` 改成你的序列埠，並在三個 `.bin` 所在資料夾執行：

```bash
esptool --chip esp32s3 --port PORT erase-flash
esptool --chip esp32s3 --port PORT --baud 460800 write-flash \
  --flash-mode qio --flash-freq 80m --flash-size 16MB \
  0x0 papercolor-hub-bootloader.bin \
  0x8000 papercolor-hub-partitions.bin \
  0x10000 papercolor-hub-firmware.bin
```

例如 Windows 將 `PORT` 改為 `COM3`；macOS／Linux 使用完整路徑。若無法連線，請拔除後
重新插入、改用資料線，並依裝置／esptool 的 Bootloader 模式說明重試。

## 4. 首次啟動

燒錄完成後重新開機。第一次彩色畫面完整刷新約需 15 秒。將
`examples/sd-card` 內容複製至 microSD 根目錄，即可使用股票、閱讀器與相框功能。

更多 ESP32-S3 燒錄參數可參考 [Espressif esptool 文件](https://docs.espressif.com/projects/esptool/en/latest/esp32s3/esptool/)。

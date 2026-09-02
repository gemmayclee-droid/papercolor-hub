# Install PaperColor Hub

[繁體中文](INSTALL.zh-TW.md) · [简体中文](INSTALL.zh-CN.md) · [English](INSTALL.en.md)

This guide is for M5Stack PaperColor C151. Back up any important device data
first: the commands below erase the existing firmware and settings.

## 1. Download a Release

Download and extract these three files from the same version on
[Releases](../../releases):

- `papercolor-hub-bootloader.bin`
- `papercolor-hub-partitions.bin`
- `papercolor-hub-firmware.bin`

## 2. Prepare the flashing tool

Only Espressif `esptool` is needed; PlatformIO is not required. Install it with
Python 3:

```bash
python -m pip install --user esptool
```

Connect PaperColor with a USB-C **data** cable. Find its serial port: normally
`COM3` on Windows, `/dev/cu.usbmodem*` on macOS, or `/dev/ttyACM0` or
`/dev/ttyUSB0` on Linux.

## 3. Erase and flash

Replace `PORT` with the device serial port and run these commands in the folder
containing the three `.bin` files:

```bash
esptool --chip esp32s3 --port PORT erase-flash
esptool --chip esp32s3 --port PORT --baud 460800 write-flash \
  --flash-mode qio --flash-freq 80m --flash-size 16MB \
  0x0 papercolor-hub-bootloader.bin \
  0x8000 papercolor-hub-partitions.bin \
  0x10000 papercolor-hub-firmware.bin
```

If the tool cannot connect, reconnect the device, use a known data cable, and
retry after putting the device into its bootloader mode as described by the
device or esptool documentation.

## 4. First boot

Restart after flashing. The first full colour refresh takes around 15 seconds.
Copy `examples/sd-card` to the microSD-card root to use stocks, reader, and
photo-frame features.

See the [Espressif esptool documentation](https://docs.espressif.com/projects/esptool/en/latest/esp32s3/esptool/) for ESP32-S3 flashing details.

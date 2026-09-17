# PaperColor Hub

[繁體中文](../README.md) · [简体中文](README.zh-CN.md) · [English](README.en.md)

PaperColor Hub is dual-mode open-source firmware for the M5Stack PaperColor
(C151): a Dropbox rotating gallery and a perpetual calendar with today's date,
weekday, and time.

Hold button C while powering on to switch between the two modes. The selection
is saved in NVS. The gallery recursively reads its configured Dropbox folder,
skips videos, displays the next photo every 12 hours, and then deep-sleeps. See
the [Dropbox gallery setup](DROPBOX.en.md).

Calendar controls: A selects the previous month, B returns to the current
month, and C selects the next month. It uses the Wi-Fi and `timezone` setting
in `/config/dropbox.json` for NTP time synchronization.

Copy this required configuration file to the microSD card:

- `/config/dropbox.json`: Dropbox credentials, Wi-Fi, timezone, gallery path,
  and refresh interval.
- `/photos/`: optional JPG, PNG, or BMP fallback photos when Dropbox is unavailable.

PlatformIO is not required on your computer. Download the three latest `.bin`
files from [Releases](../../releases), then follow the
[installation guide](INSTALL.en.md).

To build your own firmware, fork this repository and run **Build PaperColor
firmware** from GitHub **Actions**. Download its Artifact when complete. Pushing
a `v*` tag automatically creates a Release with the `.bin` files attached.

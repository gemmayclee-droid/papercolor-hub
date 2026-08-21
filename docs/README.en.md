# PaperColor Hub

[繁體中文](../README.md) · [简体中文](README.zh-CN.md) · [English](README.en.md)

PaperColor Hub is four-in-one open-source firmware for the M5Stack PaperColor
(C151): a Taiwan stock watchlist, EPUB/TXT reader, perpetual desktop calendar,
and colour e-paper photo frame.

Hold button C while powering on to select the next mode. Copy the
`examples/sd-card` layout to the microSD card:

- `/config/stocks.json`: Wi-Fi, refresh interval, and symbols. Prefix OTC
  symbols with `otc_`.
- `/Books/*.epub`: DRM-free EPUB or TXT books.
- `/fonts/*.ttf`: reading fonts, optionally with Bold/Italic siblings.
- `/photos/`: JPG, PNG, or BMP images.

Build with PlatformIO using `pio run -e m5papercolor`. Clone with
`git clone --recurse-submodules` so the FreeInk SDK is available.

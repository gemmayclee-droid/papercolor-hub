# Project history

This document was migrated from the earlier Codex task, “M5Stack PaperColor Firmware.” It retains decisions, features, and release records that remain useful to this project, without verbatim conversation, tool output, or account information.

## Origins and technical decisions

- Created `gemmayclee-droid/papercolor-hub` as an integrated firmware project for the M5Stack PaperColor C151.
- Used the official `M5Unified`, `M5GFX`, and `M5PM1` libraries for display and hardware support.
- Chose the FreeInkBook/FreeInk reader path for EPUB ZIP, CSS, CJK layout, external TTF/OTF fonts, images, bookmarks, and caching.
- Used the Taiwan Stock Exchange MIS public endpoint for Taiwan stock quotes.
- Took only product-direction inspiration from GPL-3.0 photo-frame projects; the MIT-licensed main project implements its own code.

## Feature milestones

1. Delivered four initial modes: Taiwan watchlist stocks, an EPUB/TXT reader, perpetual calendar, and color photo frame.
2. Fixed the frame's SD-file compatibility issue by loading images into PSRAM before decoding through M5GFX memory APIs, retaining JPG, PNG, and BMP support.
3. Added Sudoku as a fifth mode: A/C move, B enters digits, long B clears, and long A resets. Conflict markers and progress persist in NVS.
4. Kept UI and user documentation available in Traditional Chinese, Simplified Chinese, and English.

## Build and release

- Users do not need a local PlatformIO installation. GitHub Actions recursively obtains the FreeInk submodule, builds C151 firmware, and collects `.bin` files.
- A Git tag matching `v*` creates a GitHub Release with `firmware.bin`, `bootloader.bin`, and `partitions.bin`.
- `v0.1.0` was the first formal release. Its historical CI build succeeded, but button feel and display-refresh behavior should still be verified on hardware for every release.

## Related-project research

The earlier task also surveyed the PaperColor ecosystem: the official UserDemo, M5Unified/M5GFX/M5PM1, FreeInk SDK, ESPHome integrations, calendar, photo-frame, and information-dashboard projects. Check the current state and license of every upstream project before adopting it.

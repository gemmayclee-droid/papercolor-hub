# 專案歷史

本文件由先前的 Codex 任務「M5Stack PaperColor 韌體」移入，保留對目前專案仍有價值的決策、功能與發布紀錄；不含逐字對話、工具輸出或帳號資訊。

## 起源與技術決策

- 建立 `gemmayclee-droid/papercolor-hub`，作為 M5Stack PaperColor C151 的整合式韌體。
- 顯示與硬體採用官方 `M5Unified`、`M5GFX` 與 `M5PM1`。
- 閱讀器採 FreeInkBook／FreeInk 技術路線，以支援 EPUB ZIP、CSS、CJK 排版、外部 TTF/OTF 字型、圖片、書籤及快取。
- 台股行情採臺灣證券交易所 MIS 公開端點。
- 相框只借鑑 GPL-3.0 專案的功能方向，主專案自行實作，以維持 MIT 授權相容性。

## 功能里程碑

1. 完成四個初始模式：台灣自選股、EPUB/TXT 閱讀器、桌面萬年曆與彩色相框。
2. 修正相框的 SD 檔案介面相容問題：影像先讀入 PSRAM，再交由 M5GFX 的記憶體解碼 API 顯示，保留 JPG、PNG 與 BMP 支援。
3. 新增第五個模式「數獨」：A/C 移動、B 填數、長按 B 清除、長按 A 重設；衝突標示與進度保存於 NVS。
4. 所有介面與使用說明同步提供繁體中文、簡體中文與英文。

## 建置與發布

- 不要求使用者在本機安裝 PlatformIO；GitHub Actions 負責遞迴取得 FreeInk 子模組、編譯 C151 韌體及收集 `.bin`。
- Git tag 符合 `v*` 時，工作流程會建立 GitHub Release，附上 `firmware.bin`、`bootloader.bin` 與 `partitions.bin`。
- `v0.1.0` 已建立為首個正式 Release；歷史任務記錄中線上編譯成功，但實機按鍵手感與畫面刷新仍應以每次硬體測試為準。

## 相關資源調查

先前也整理過 PaperColor 生態系：官方 UserDemo、M5Unified/M5GFX/M5PM1、FreeInk SDK、ESPHome 整合、萬年曆、相框及資訊面板等。實作時應優先確認各上游專案的最新狀態與授權。

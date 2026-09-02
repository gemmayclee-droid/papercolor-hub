# PaperColor Hub

[繁體中文](README.md) · [简体中文](docs/README.zh-CN.md) · [English](docs/README.en.md)

PaperColor Hub 是 M5Stack PaperColor（C151）的五合一開源韌體：

1. 台灣股票自選行情（TWSE MIS）
2. EPUB／TXT 閱讀器與外部 TTF／OTF 字型
3. 桌面萬年曆
4. 彩色電子紙相框
5. 數獨遊戲

## 操作

關機後按住 C 鍵（GPIO 1）再開機，每次會切換至下一個模式。選擇會保存在
NVS；正常開機會直接進入上次模式。

- 股票：B 立即更新。
- 閱讀器：A／B 翻頁，C 確認或開啟目錄；沿用 FreeInk Reader 的按鍵操作。
- 萬年曆：A 上個月、B 回今天、C 下個月。
- 相框：A 上一張、B 重新掃描、C 下一張；每五分鐘自動輪播。
- 數獨：A／C 移動至上一／下一個可填格，B 依序填入 1–9；長按 B 清除，
  長按 A 重設題目。紅字表示與同行、同列或同宮衝突，進度會自動保存。

## microSD 目錄

先將 `examples/sd-card` 內的內容複製到 microSD 根目錄：

```text
/config/stocks.json
/Books/*.epub
/fonts/*.ttf
/photos/*.{jpg,jpeg,png,bmp}
/BookCache/                 # 閱讀器自動建立
```

台股代號預設使用上市市場，例如 `2330` 等同 `tse_2330`。上櫃股票請明確寫成
`otc_6488`。行情來自臺灣證券交易所 MIS 公開端點；請遵守其使用條款，這不是
交易系統，也不保證資料即時性或完整性。

只支援無 DRM 的 EPUB。字型可直接放入 `/fonts`；同名字型的粗體、斜體版本會
自動形成 fallback family。閱讀器引擎源自 CrossPoint 技術路線的 FreeInk，
支援 EPUB ZIP、CSS、CJK 斷行、雙向文字、圖片、書籤和頁面快取。

## 安裝與自行建置

不需要在電腦安裝 PlatformIO。前往 [Releases](../../releases) 下載最新版本的三個
`.bin` 檔，再依照[安裝說明](docs/INSTALL.zh-TW.md)燒錄。

若要自行建置，將 repository fork 到自己的 GitHub 帳號後，從 GitHub 的 **Actions**
頁面執行 **Build PaperColor firmware**；完成後可從 Artifact 下載產物。推送
`v*` 格式的 tag 時，GitHub Actions 會自動建立 Release 並附上 `.bin` 檔。

目前針對 PaperColor C151（ESP32-S3R8、16 MB Flash、8 MB OPI PSRAM、600×400
Spectra 6）建置。第一次完整彩色刷新通常需要約 15 秒；閱讀模式使用 FreeInk
的快速單色刷新路徑，並需要定期完整刷新以保持面板電荷平衡。

## 授權

主專案採 MIT License。第三方程式與來源說明見
[`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md)。

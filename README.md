# PaperColor Hub

[繁體中文](README.md) · [简体中文](docs/README.zh-CN.md) · [English](docs/README.en.md) · [Dropbox 藝廊設定](docs/DROPBOX.zh-TW.md) · [專案歷史](docs/HISTORY.zh-TW.md)

PaperColor Hub 是 M5Stack PaperColor（C151）的雙模式開源韌體：

1. Dropbox 循環藝廊：遞迴讀取指定資料夾與子資料夾、略過影片，只顯示照片；預設每 12 小時更新一次。
2. 萬年曆：顯示今日日期、星期與時間，並可瀏覽前後月份。

詳見 [Dropbox 藝廊設定](docs/DROPBOX.zh-TW.md)。

## 操作

關機後按住 C 鍵（GPIO 1）再開機，可在 Dropbox 藝廊與萬年曆之間切換。選擇會保存在 NVS；正常開機會直接進入上次模式。

- Dropbox 藝廊：同步後即進入深度睡眠；首次或同步失敗時，顯示 microSD 的 `/photos` 備援照片。
- 萬年曆：A 上個月、B 回本月、C 下個月；標題會顯示目前日期、星期與時間，且每分鐘更新。

## microSD 目錄

先將 `examples/sd-card` 內的內容複製到 microSD 根目錄：

```text
/config/dropbox.json
/photos/*.{jpg,jpeg,png,bmp} # Dropbox 失敗時的備援照片
```

萬年曆會使用 `/config/dropbox.json` 中的 Wi-Fi 與 `timezone` 設定，在開啟模式時透過 NTP 校正時間；預設時區為 `CST-8`（台灣／中國標準時間）。

## 安裝與自行建置

不需要在電腦安裝 PlatformIO。前往 [Releases](../../releases) 下載最新版本的三個
`.bin` 檔，再依照[安裝說明](docs/INSTALL.zh-TW.md)燒錄。

若要自行建置，將 repository fork 到自己的 GitHub 帳號後，從 GitHub 的 **Actions**
頁面執行 **Build PaperColor firmware**；完成後可從 Artifact 下載產物。推送
`v*` 格式的 tag 時，GitHub Actions 會自動建立 Release 並附上 `.bin` 檔。

目前針對 PaperColor C151（ESP32-S3R8、16 MB Flash、8 MB OPI PSRAM、600×400
Spectra 6）建置。完整彩色刷新通常需要約 15 秒，因此 Dropbox 藝廊採低頻同步與深度睡眠設計。

## 授權

主專案採 MIT License。第三方程式與來源說明見
[`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md)。

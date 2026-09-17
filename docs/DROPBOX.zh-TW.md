# Dropbox 循環藝廊設定

相框模式可直接連線 Dropbox，不需中介伺服器。它會遞迴讀取指定資料夾與所有子資料夾，只選取照片，略過影片及其他檔案；每次喚醒後顯示循環中的下一張，再進入深度睡眠。

## 行為

- 預設每 12 小時更新一次。
- 支援 JPG、JPEG、PNG 與 BMP。影片（例如 MP4、MOV）會略過。
- 相片依 Dropbox 路徑排序，最後一張後回到第一張。
- 播放位置保存於 NVS；新照片加入或裝置重開後仍會接續循環。
- 下載時使用 Dropbox 縮圖 API，並儲存為適合 PaperColor 顯示的 JPEG 快取。
- Wi-Fi、Dropbox 或下載失敗時，保留既有畫面並在下一個更新週期重試。

## 設定檔

將範例檔 [dropbox.json](../examples/sd-card/config/dropbox.json) 複製到 microSD 的 `/config/dropbox.json`，再填入資料：

```json
{
  "wifi": { "ssid": "YOUR_WIFI_SSID", "password": "YOUR_WIFI_PASSWORD" },
  "dropbox": {
    "app_key": "YOUR_DROPBOX_APP_KEY",
    "refresh_token": "YOUR_DROPBOX_OFFLINE_REFRESH_TOKEN",
    "folder": "/Apps/PaperColor Gallery"
  },
  "refresh_seconds": 43200
}
```

`folder` 可指向任何你已授權的 Dropbox 資料夾；建議使用 App folder 權限，將照片放在 `/Apps/PaperColor Gallery/` 下。子資料夾可自由分類，裝置會遞迴處理。

## Dropbox 授權

請在 Dropbox App Console 建立 App，啟用檔案中繼資料與內容讀取權限，並使用 OAuth Code Flow with PKCE 取得離線 refresh token。不要把 Dropbox 密碼或短期 access token 寫入 SD 卡；韌體會用 refresh token 自行取得短期 access token。

這是直接由 PaperColor 對 Dropbox API 發出請求的設計；Dropbox App Key 與 refresh token 只存放在你的 microSD 與裝置設定範圍內。

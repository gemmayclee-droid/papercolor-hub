# Dropbox 循环画廊设置

相框模式可直接连接 Dropbox，无需中间服务器。它会递归读取指定文件夹及所有子文件夹，只选择照片，跳过视频和其他文件；每次唤醒后显示循环中的下一张，再进入深度睡眠。

## 行为

- 默认每 12 小时更新一次。
- 支持 JPG、JPEG、PNG 和 BMP。视频（例如 MP4、MOV）会跳过。
- 照片按 Dropbox 路径排序，最后一张后回到第一张。
- 播放位置保存于 NVS；新增照片或设备重启后仍会接续循环。
- 下载时使用 Dropbox 缩略图 API，并保存为适合 PaperColor 显示的 JPEG 缓存。
- Wi-Fi、Dropbox 或下载失败时，保留既有画面并在下一个更新周期重试。

## 配置文件

将示例文件 [dropbox.json](../examples/sd-card/config/dropbox.json) 复制到 microSD 的 `/config/dropbox.json`，再填入资料：

```json
{
  "wifi": { "ssid": "YOUR_WIFI_SSID", "password": "YOUR_WIFI_PASSWORD" },
  "dropbox": {
    "app_key": "YOUR_DROPBOX_APP_KEY",
    "refresh_token": "YOUR_DROPBOX_OFFLINE_REFRESH_TOKEN",
    "folder": "/Apps/PaperColor Gallery"
  },
  "timezone": "CST-8",
  "refresh_seconds": 43200
}
```

`folder` 可指向任何已授权的 Dropbox 文件夹；建议使用 App folder 权限，并将照片放在 `/Apps/PaperColor Gallery/` 下。子文件夹可自由分类，设备会递归处理。

`timezone` 使用 POSIX 时区格式；台湾、中国标准时间可使用 `CST-8`。万年历模式会使用此设置通过 NTP 校正并显示今天日期、星期和时间。

## Dropbox 授权

请在 Dropbox App Console 创建 App，启用文件元数据与内容读取权限，并使用 OAuth Code Flow with PKCE 取得离线 refresh token。不要把 Dropbox 密码或短期 access token 写入 SD 卡；固件会用 refresh token 自行取得短期 access token。

这是由 PaperColor 直接向 Dropbox API 发出请求的设计；Dropbox App Key 与 refresh token 只存放在你的 microSD 与设备设置范围内。

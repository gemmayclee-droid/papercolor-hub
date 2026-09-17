# Dropbox rotating gallery setup

Photo Frame mode can connect directly to Dropbox without an intermediary service. It recursively reads the configured folder and its subfolders, selects only photos, skips videos and other files, displays the next photo in sequence after each wake, and then returns to deep sleep.

## Behavior

- Updates every 12 hours by default.
- Supports JPG, JPEG, PNG, and BMP. Videos such as MP4 and MOV are skipped.
- Photos are ordered by Dropbox path; the sequence wraps after the last item.
- The current position is kept in NVS, so it continues after a reboot or folder changes.
- Downloads use the Dropbox thumbnail API and cache a JPEG sized appropriately for PaperColor.
- On Wi-Fi, Dropbox, or download failure, the existing screen remains and the device retries on the next cycle.

## Configuration

Copy the example [dropbox.json](../examples/sd-card/config/dropbox.json) to `/config/dropbox.json` on the microSD card and fill it in:

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

`folder` can point to any Dropbox folder you authorized. App-folder access is recommended; place photos under `/Apps/PaperColor Gallery/`. Subfolders may be organized freely and are processed recursively.

`timezone` uses POSIX timezone notation; `CST-8` is suitable for Taiwan and China Standard Time. Calendar mode uses this setting to synchronize NTP time and show today's date, weekday, and time.

## Dropbox authorization

Create an app in the Dropbox App Console, enable file-metadata and content-read scopes, then use OAuth Code Flow with PKCE to obtain an offline refresh token. Do not put a Dropbox password or short-lived access token on the SD card; the firmware uses the refresh token to obtain short-lived access tokens itself.

PaperColor makes these API requests directly to Dropbox. The Dropbox App Key and refresh token remain within your microSD and device setup scope.

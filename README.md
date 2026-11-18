# DashKit Firmware

CAN-to-BLE bridge firmware for ESP32-S3. Reads CAN bus via MCP2518FD and streams frames to the [DashPilot](https://github.com/softwiredtech/dashpilot) mobile app over Bluetooth Low Energy.

## Prerequisites

- macOS with [Homebrew](https://brew.sh)
- USB cable for flashing

## Setup

### 1. Install dependencies

```bash
brew install cmake ninja dfu-util
```

### 2. Clone and set up ESP-IDF

ESP-IDF v5.4.4 is included as a git submodule:

```bash
git submodule update --init --recursive
./esp-idf/install.sh esp32s3
```

### 3. Activate ESP-IDF environment

Run this in every new terminal session (or add it to your `~/.zshrc`):

```bash
. ./esp-idf/export.sh
```

## Build & Flash

```bash
idf.py set-target esp32s3
idf.py build
idf.py flash monitor
```

Press `Ctrl+]` to exit the serial monitor.

## GitHub releases

Pushing a new tag (for example, `git push origin v0.3.2`) runs
`.github/workflows/release.yml`. It builds the tagged source with the pinned
ESP-IDF submodule and publishes the firmware to a GitHub release:

- `dashkit-firmware.bin`: application binary for OTA updates.
- `dashkit-firmware-usb.bin`: complete flash image including the bootloader
  and partition table; flash this image
  at address `0x0` over USB.
- `SHA256SUMS.txt`: binary checksums.

The release is published only after the build succeeds. Build artifacts are also
available on the workflow run for 14 days; release assets remain available
until the release or assets are deleted. Re-running the workflow replaces the
assets on an existing release for that tag.

Release notes list commit subjects and short hashes since the nearest previous
tag in the release commit's ancestry, oldest first. The first release includes
the full commit history. Re-running the workflow also refreshes these notes.

## Debug Logging

To enable verbose CAN frame and status logging, add to `sdkconfig.defaults`:

```
CONFIG_DASHKIT_DEBUG_LOG=y
```

Then rebuild:

```bash
idf.py fullclean && idf.py build
```

## Hardware

| Function | GPIO |
|----------|------|
| SPI MOSI | 11 |
| SPI SCLK | 12 |
| SPI MISO | 13 |
| SPI CS | 10 |
| CAN INT | 9 |
| LED Red | 39 |
| LED Green | 40 |
| LED Blue | 41 |

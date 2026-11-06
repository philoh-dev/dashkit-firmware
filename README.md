# DashKit Firmware

CAN-to-BLE bridge firmware for ESP32-S3. Reads CAN bus via MCP2518FD and streams frames to the [DashPilot](https://github.com/softwiredtech/dashpilot) mobile app over Bluetooth Low Energy.

## Interior ambient lighting

Vehicle CAN (bus 1) ID **0x679**, `UI_ambientLightingCtrls`, is forwarded
unchanged over the CAN BLE stream. This uses one additional hardware filter
slot (21 of 32 on the vehicle controller). The message is filter-only; the app
decodes it. Apps installing a nonempty CAN filter must include bus 1 / 0x679
alongside their other entries.

The supplied newer Tesla database layout uses zero-based byte offsets:

| Field | Location |
| --- | --- |
| Enable/effect, raw | Byte 0 |
| Red, green, blue | Bytes 1, 2, 3 |
| Brightness, 0-100% | Byte 4, lower seven bits |
| Target sections, raw | Bits 43-48 |

The section mask is `((data[5] >> 3) | (data[6] << 5)) & 0x3f`.
The DBC assumes a nominal eight-byte payload; forwarding preserves the actual
received length and all bytes. RGB forwarding to a Rive dashboard has been
confirmed working on a user's vehicle. Effect subdivisions, section labels,
and compatibility with other vehicle/software variants remain unverified.

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

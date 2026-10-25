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


### Read-only CAN mode

Before building and flashing, run:

```bash
idf.py menuconfig
```

Under **DashKit**, enable **Read-only CAN bus (listen only, no transmissions)**.
Save, then run `idf.py build flash monitor`. For normal operation, disable
the option and rebuild/flash. The default remains read/write.

For a separate read-only build using the included preset:

```bash
idf.py -B build-read-only -D SDKCONFIG=build-read-only/sdkconfig -D "SDKCONFIG_DEFAULTS=sdkconfig.defaults;sdkconfig.read-only" build flash monitor
```

Use a fresh build directory for this preset; defaults do not override an
existing sdkconfig. For an existing configuration, use menuconfig.
Read-only firmware sets both CAN controllers to hardware listen-only mode:
no transmitted frames, acknowledgements, or error frames. It skips startup
loopback tests and rejects CAN send requests from controls and automations
with `ESP_ERR_NOT_SUPPORTED`. CAN reception and BLE streaming remain enabled.
A listen-only device needs other active CAN nodes to acknowledge traffic.

This is a firmware build setting, not a runtime toggle. Any later USB or OTA
image must also be built with `CONFIG_DASHKIT_CAN_READ_ONLY=y` to preserve it.
Tesla BLE vehicle commands are separate; disable **Tesla BLE vehicle-command
client** in the same menu if those commands should also be unavailable.

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

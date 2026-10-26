# DashKit Firmware

CAN-to-BLE bridge firmware for ESP32-S3. Reads CAN bus via MCP2518FD and streams frames to the [DashPilot](https://github.com/softwiredtech/dashpilot) mobile app over Bluetooth Low Energy.

## Two connected devices

Two paired app devices can receive CAN data simultaneously, alongside the
independent Tesla BLE connection. The first eligible encrypted CAN subscriber becomes
the controller. Connecting or subscribing a second device keeps the current
controller; the other eligible subscriber takes over when it disconnects,
unsubscribes, or marks itself read-only.

A device opts into read-only mode by writing **`4F 01`** (two bytes) to the
encrypted control characteristic **CADA0004-CA00-B1E0-B0D6-C000AA0100A1**.
Send this as soon as the link is encrypted, before enabling CAN notifications.
Bonded peers may have subscriptions restored automatically; the flag immediately
releases any controller role acquired during reconnect. The other subscribed device
automatically gets read/write control. Read-only devices continue receiving
data and cannot take control even if the other device leaves.

Write **`4F 00`** to become eligible for control again; an existing controller
keeps control. The flag applies only to the sender's current connection and
must be sent again after every reconnect. Devices that omit the flag are
eligible for control, preserving compatibility with existing apps. If both
devices are eligible, the first subscriber controls; if both opt into read-only
mode, neither can send commands.

Both devices receive the same CAN stream using the controller's CAN filter.
Each link uses its own negotiated MTU. Tesla status notifications go to every
subscriber of the status characteristic, including devices without a CAN subscription.
Only the controller can change the CAN filter, send vehicle or Tesla enrollment
commands, or update firmware. Keepalive pings are accepted from both devices.
Firmware updates pause CAN streaming on both links and abort if control changes.

Advertising continues until both app slots are occupied. To pair the second
device, open the existing pairing window from the controller. If both slots
are occupied, pairing mode disconnects the other device to make room.

Run the host regression tests with `bash tools/test/run_ble_clients_test.sh`.

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

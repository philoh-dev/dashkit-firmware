#!/usr/bin/env bash
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
mkdir -p "${REPO_ROOT}/build/ble-host-test"
cc -std=c11 -Wall -Wextra -Werror -I "${REPO_ROOT}/main/ble" \
   "${SCRIPT_DIR}/test_ble_clients.c" -o "${REPO_ROOT}/build/ble-host-test/test_ble_clients"
"${REPO_ROOT}/build/ble-host-test/test_ble_clients"
cc -std=c11 -Wall -Wextra -Werror -DCONFIG_DASHKIT_READ_ONLY=1 \
   -I "${REPO_ROOT}/main/ble" "${SCRIPT_DIR}/test_ble_read_only.c" \
   -o "${REPO_ROOT}/build/ble-host-test/test_ble_read_only"
"${REPO_ROOT}/build/ble-host-test/test_ble_read_only"
echo "BLE write-permission and MTU tests passed"

// Compile with -DCONFIG_DASHKIT_READ_ONLY=1, then run on the host.
#include "ble_clients.h"

#define CHECK(expr) do { if (!(expr)) return __LINE__; } while (0)

int main(void)
{
    ble_app_conn_t peers[BLE_APP_MAX_CONNS] = {0};
    for (size_t i = 0; i < BLE_APP_MAX_CONNS; i++) {
        peers[i].handle = (uint16_t)i;
        peers[i].encrypted = true;
        peers[i].subscribed = true;
        CHECK(!ble_app_can_write(&peers[i]));
        CHECK(ble_app_can_receive(&peers[i]));
        peers[i].read_only = true;
        CHECK(!ble_app_can_write(&peers[i]));
        // Even a cleared session flag cannot override the build policy.
        peers[i].read_only = false;
        CHECK(!ble_app_can_write(&peers[i]));
    }
    CHECK(ble_app_has_receivers(peers));
    peers[0].handle = BLE_APP_NO_CONN;
    CHECK(ble_app_has_receivers(peers));
    CHECK(!ble_app_can_write(&peers[1]));
    return 0;
}

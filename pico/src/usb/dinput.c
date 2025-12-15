#include <stdbool.h>

#include "bsp/board_api.h"
#include "device/usbd_pvt.h"
#include "led.h"
#include "tusb.h"
#include "usb_descriptors.h"

void send_hid_report(int btn) {
    // use to avoid send multiple consecutive zero reports
    static bool has_gamepad_key = false;

    hid_gamepad_report_t report = {.x = 0,
                                   .y = 0,
                                   .z = 0,
                                   .rz = 0,
                                   .rx = 0,
                                   .ry = 0,
                                   .hat = 0,
                                   .buttons = 0};

    if (btn) {
        report.buttons |= GAMEPAD_BUTTON_A;
        tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
        has_gamepad_key = true;
    } else {
        if (has_gamepad_key)
            tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
        has_gamepad_key = false;
    }
}

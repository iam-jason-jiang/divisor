#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board_api.h"
#include "device/usbd_pvt.h"
#include "led.h"
#include "tusb.h"
#include "usb_callbacks.h"
#include "usb_descriptors.h"
#include "gamepad.h"



void usb_task(controller_state_t *controler_state) {
    // Poll every 10ms
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms) return;  // not enough time
    start_ms += interval_ms;

    uint32_t const btn = board_button_read();

    // remote wakeup
    if (tud_suspended() && btn) {
        tud_remote_wakeup();
    }

    // dont send report if usb not ready
    if (!tud_ready()) {
        return;
    }

    switch (input_mode) {
        case D_INPUT:
            send_hid_report(btn);
            break;
        default:
        case X_INPUT:
            send_xinput_report(btn);
            break;
    }
}

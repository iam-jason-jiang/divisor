#include "gamepad.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board_api.h"
#include "device/usbd_pvt.h"
#include "led.h"
#include "tusb.h"
#include "usb_callbacks.h"
#include "usb_descriptors.h"

static bool active_input_in_last_cycle = false;

// TODO: EXPERIMENTAL VALUE
#define ANALOG_DEAD_ZONE 200

static bool is_any_input_active(controller_state_t const *state) {
    if (state->button_a || state->button_b || state->button_x || state->button_y ||
        state->dpad_up || state->dpad_down || state->dpad_left || state->dpad_right) {
        return true;
    }

    int ldx = (int)state->left_stick_x - 2048;
    int ldy = (int)state->left_stick_y - 2048;
    if (ldx * ldx + ldy * ldy > ANALOG_DEAD_ZONE) {
        return true;
    }

    int rdx = (int)state->right_stick_x - 2048;
    int rdy = (int)state->right_stick_y - 2048;
    if (rdx * rdx + rdy * rdy > ANALOG_DEAD_ZONE) {
        return true;
    }

    return false;
}

void usb_task(controller_state_t *controller_state) {
    // Poll every 10ms
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms) return;  // not enough time
    start_ms += interval_ms;

    bool input_is_active = is_any_input_active(controller_state);

    // avoid sending multiple consecutive zero reports
    if (!input_is_active && !active_input_in_last_cycle) {
        return;
    }

    // remote wakeup
    if (tud_suspended() && input_is_active) {
        tud_remote_wakeup();
    }

    // dont send report if usb not ready
    if (!tud_ready()) {
        return;
    }

    switch (input_mode) {
        case D_INPUT:
            send_hid_report(controller_state);
            break;
        default:
        case X_INPUT:
            send_xinput_report(controller_state);
            break;
    }

    // Update the state for the next cycle
    active_input_in_last_cycle = input_is_active;
}

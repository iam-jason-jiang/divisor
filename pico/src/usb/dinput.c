#include <stdbool.h>

#include "bsp/board_api.h"
#include "device/usbd_pvt.h"
#include "led.h"
#include "tusb.h"
#include "usb_descriptors.h"

// Define constants for the 4 direction inputs
#define DPAD_UP_MASK    (1 << 0) // Bit 0
#define DPAD_DOWN_MASK  (1 << 1) // Bit 1
#define DPAD_LEFT_MASK  (1 << 2) // Bit 2
#define DPAD_RIGHT_MASK (1 << 3) // Bit 3

uint8_t convert_dpad_to_hat(bool up, bool down, bool left, bool right) {
    uint8_t dpad_mask = 0;

    if (up) dpad_mask |= DPAD_UP_MASK;
    if (down) dpad_mask |= DPAD_DOWN_MASK;
    if (left) dpad_mask |= DPAD_LEFT_MASK;
    if (right) dpad_mask |= DPAD_RIGHT_MASK;

    switch (dpad_mask) {
        case DPAD_UP_MASK:
            return GAMEPAD_HAT_UP;
        case DPAD_DOWN_MASK:
            return GAMEPAD_HAT_DOWN;
        case DPAD_LEFT_MASK:
            return GAMEPAD_HAT_LEFT;
        case DPAD_RIGHT_MASK:
            return GAMEPAD_HAT_RIGHT;

        // --- Diagonal Combinations ---
        case DPAD_UP_MASK | DPAD_RIGHT_MASK:
            return GAMEPAD_HAT_UP_RIGHT;
        case DPAD_DOWN_MASK | DPAD_RIGHT_MASK:
            return GAMEPAD_HAT_DOWN_RIGHT;
        case DPAD_DOWN_MASK | DPAD_LEFT_MASK:
            return GAMEPAD_HAT_DOWN_LEFT;
        case DPAD_UP_MASK | DPAD_LEFT_MASK:
            return GAMEPAD_HAT_UP_LEFT;

        // --- Conflict Resolution & Default ---
        case DPAD_UP_MASK | DPAD_DOWN_MASK:
        case DPAD_LEFT_MASK | DPAD_RIGHT_MASK:
        default:
            return GAMEPAD_HAT_CENTERED;
    }
}

void send_hid_report(controller_state_t const *state) {
    hid_gamepad_report_t report = {
        .x = state->left_stick_x >> 4,
        .y = state->left_stick_y >> 4,
        .z = state->right_stick_x >> 4,
        .rz = state->right_stick_y >> 4,
        .rx = 0,
        .ry = 0,
        .hat = 0,
        .buttons = (state->button1 ? GAMEPAD_BUTTON_A : 0) |  // A
                   (state->button2 ? GAMEPAD_BUTTON_B : 0) |  // B
                   (state->button3 ? GAMEPAD_BUTTON_X : 0) |  // X
                   (state->button4 ? GAMEPAD_BUTTON_Y : 0),   // Y
    };

    report.hat = convert_dpad_to_hat(state->button5, state->button6,
                                     state->button7, state->button8);

    tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
}

// callback functions

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen) {
    // TODO not Implemented
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize) {
    // The function is intentionally empty,
    // as we do not expect any output reports for a dedicated gamepad.
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)bufsize;
}

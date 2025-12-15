#include <stdbool.h>

#include "bsp/board_api.h"
#include "device/usbd_pvt.h"
#include "led.h"
#include "tusb.h"
#include "usb_descriptors.h"

void send_hid_report(int btn) {
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
    }

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board_api.h"
#include "gamepad.h"
#include "led.h"
#include "system_context.h"
#include "tusb.h"
#include "usb_callbacks.h"
#include "usb_descriptors.h"

uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

// set default input mode to XInput
int input_mode = X_INPUT;

int main(void) {
    board_init();
    pico_led_init();

    // init device stack on configured roothub port
    tud_init(BOARD_TUD_RHPORT);

    if (board_init_after_tusb) {
        board_init_after_tusb();
    }

    controller_state_t controller_state = {0};
    controller_state.left_stick_x = 2048;
    controller_state.left_stick_y = 2048;
    controller_state.right_stick_x = 2048;
    controller_state.right_stick_y = 2048;

    while (1) {
        tud_task();
        led_blinking_task();

        uint32_t const btn = board_button_read();
        controller_state.button_a = (btn) ? 1 : 0;
        controller_state.left_stick_x = (btn) ? 4095 : 2048;

        usb_task(&controller_state);
    }
}

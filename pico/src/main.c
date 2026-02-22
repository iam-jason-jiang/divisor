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
    system_context_t ctx = {0};

    board_init();
    pico_led_init();

    gamepad_init_hw();

    // init device stack on configured roothub port
    tud_init(BOARD_TUD_RHPORT);

    if (board_init_after_tusb) {
        board_init_after_tusb();
    }

    ctx.gamepad.left_stick_x = 2048;
    ctx.gamepad.left_stick_y = 2048;
    ctx.gamepad.right_stick_x = 2048;
    ctx.gamepad.right_stick_y = 2048;

    while (1) {
        tud_task();
        led_blinking_task();

        poll_right_inputs(&ctx.gamepad);
        ctx.gamepad.button_b = board_button_read();

        usb_task(&ctx.gamepad);
    }
}

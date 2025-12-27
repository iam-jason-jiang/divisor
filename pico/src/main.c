/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

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

controller_state_t controller_state = {0};

void update_left(system_context_t *ctx) {
    poll_left_inputs(&ctx->gamepad);

    // TODO: send data to right side UART
}

void update_right(system_context_t *ctx) {
    tud_task();
    led_blinking_task();

    poll_right_inputs(&ctx->gamepad);
    // TODO: receive data from left side UART

    usb_task(&ctx->gamepad);
}

int main(void) {
    system_context_t ctx = {0};

    board_init();
    pico_led_init();

    const uint32_t LOCK_TIMEOUT_MS = 500;
    uint32_t start_time = to_ms_since_boot(get_absolute_time());

    // init device stack on configured roothub port
    tud_init(BOARD_TUD_RHPORT);

    if (board_init_after_tusb) {
        board_init_after_tusb();
    }

    // default to left side device
    void (*update_task)(system_context_t *) = update_left;

    // wait for usb device
    while (to_ms_since_boot(get_absolute_time()) - start_time <
           LOCK_TIMEOUT_MS) {
        tud_task();
        if (tud_ready()) {
            update_task = update_right;
            break;
        }
    }

    while (1) {
        update_task(&ctx);
    }
}

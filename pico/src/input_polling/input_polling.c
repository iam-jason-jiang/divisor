#include "bsp/board_api.h"

#include "gamepad.h"

void poll_right_inputs(controller_state_t *controller_state) {
    uint32_t const btn = board_button_read();
    controller_state->button1 = (btn) ? 1 : 0;
    controller_state->right_stick_x = (btn) ? 4095 : 2048;
}

void poll_left_inputs(controller_state_t *controller_state) {
    uint32_t const btn = board_button_read();
    controller_state->button8 = (btn) ? 1 : 0;
    controller_state->left_stick_x = (btn) ? 4095 : 2048;
}

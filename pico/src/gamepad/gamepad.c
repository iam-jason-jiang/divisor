#include "gamepad.h"

#include "bsp/board_api.h"

#define BTN_A_PIN 1
#define BTN_B_PIN 2
#define BTN_X_PIN 3
#define BTN_Y_PIN 4
#define UP_PIN 5
#define DOWN_PIN 6
#define LEFT_PIN 7
#define RIGHT_PING 8

void gamepad_init_hw(void) {
    // 1. Initialize the pins
    gpio_init(BTN_A_PIN);
    gpio_init(BTN_B_PIN);
    gpio_init(BTN_X_PIN);
    gpio_init(BTN_Y_PIN);
    gpio_init(UP_PIN);
    gpio_init(DOWN_PIN);
    gpio_init(LEFT_PIN);
    gpio_init(RIGHT_PING);

    // 2. Set as input
    gpio_set_dif(BTN_A_PIN);
    gpio_set_dif(BTN_B_PIN);
    gpio_set_dif(BTN_X_PIN);
    gpio_set_dif(BTN_Y_PIN);
    gpio_set_dif(UP_PIN);
    gpio_set_dif(DOWN_PIN);
    gpio_set_dif(LEFT_PIN);
    gpio_set_dif(RIGHT_PING);

    // 3. all pins are pulled up
    gpio_pull_up(BTN_A_PIN);
    gpio_pull_up(BTN_B_PIN);
    gpio_pull_up(BTN_X_PIN);
    gpio_pull_up(BTN_Y_PIN);
    gpio_pull_up(UP_PIN);
    gpio_pull_up(DOWN_PIN);
    gpio_pull_up(LEFT_PIN);
    gpio_pull_up(RIGHT_PING);
}

void poll_right_inputs(controller_state_t *controller_state) {
    uint32_t const btn = board_button_read();

    // TEMP center joysticks
    controller_state->right_stick_x = 2048;
    controller_state->right_stick_y = 2048;

    controller_state->button_a = (btn) ? 1 : 0;
    controller_state->right_stick_x = (btn) ? 4095 : 2048;
}

void poll_left_inputs(controller_state_t *controller_state) {
    uint32_t const btn = board_button_read();

    // TEMP center joysticks
    controller_state->left_stick_x = 2048;
    controller_state->left_stick_y = 2048;

    controller_state->dpad_right = (btn) ? 1 : 0;
    controller_state->left_stick_x = (btn) ? 4095 : 2048;
}

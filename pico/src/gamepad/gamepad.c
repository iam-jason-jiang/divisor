#include "gamepad.h"

#include "bsp/board_api.h"
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

enum {
    BTN_INDEX_A,
    BTN_INDEX_B,
    BTN_INDEX_X,
    BTN_INDEX_Y,
};

enum {
    BTN_INDEX_UP,
    BTN_INDEX_DOWN,
    BTN_INDEX_LEFT,
    BTN_INDEX_RIGHT,
};

static const uint8_t BUTTON_PINS[] = {
    2,  // A / UP
    3,  // B / DOWN
    4,  // X / LEFT
    5   // Y / RIGHT
};
static const size_t NUM_BUTTONS = sizeof(BUTTON_PINS) / sizeof(BUTTON_PINS[0]);

#define PIN_JOYSTICK_X 26
#define PIN_JOYSTICK_Y 27

void gamepad_init_hw(void) {
    // buttons
    for (size_t i = 0; i < NUM_BUTTONS; i++) {
        uint32_t gpio = BUTTON_PINS[i];

        gpio_init(gpio);
        gpio_set_dir(gpio, GPIO_IN);
        gpio_pull_up(gpio);
    }

    // ADC
    // adc_init();
    // adc_gpio_init(PIN_JOYSTICK_X);
    // adc_gpio_init(PIN_JOYSTICK_Y);
}

int16_t read_joystick(uint8_t channel) {
    adc_select_input(channel);
    
    uint32_t sum = 0;
    for (int i = 0; i < 16; i++) {
        sum += adc_read(); 
    }

    uint32_t scaled_unsigned = (sum * 65551) >> 16;

    // Centering to Signed 16-bit
    // Shift the unsigned 0..65535 range to signed -32768..32767
    int32_t centered = (int32_t)scaled_unsigned - 32768;

    return (int16_t)centered;
}

void poll_right_inputs(controller_state_t *controller_state) {
    // uint32_t const btn = board_button_read();

    // TEMP center joysticks
    // controller_state->right_stick_x = 2048;
    // controller_state->right_stick_y = 2048;

    // controller_state->button_a = (btn) ? 1 : 0;
    // controller_state->right_stick_x = (btn) ? 4095 : 2048;

    controller_state->button_a = !gpio_get(BUTTON_PINS[BTN_INDEX_A]);
    controller_state->button_b = !gpio_get(BUTTON_PINS[BTN_INDEX_B]);
    controller_state->button_x = !gpio_get(BUTTON_PINS[BTN_INDEX_X]);
    controller_state->button_y = !gpio_get(BUTTON_PINS[BTN_INDEX_Y]);

    // potentially sample ADC multiple times
    // with read_joystick()
    // adc_select_input(0);
    // controller_state->right_stick_x = adc_read();
    // adc_select_input(1);
    // controller_state->right_stick_y = adc_read();
}

void poll_left_inputs(controller_state_t *controller_state) {
    // uint32_t const btn = board_button_read();

    // TEMP center joysticks
    // controller_state->left_stick_x = 2048;
    // controller_state->left_stick_y = 2048;

    // controller_state->dpad_right = (btn) ? 1 : 0;
    // controller_state->left_stick_x = (btn) ? 4095 : 2048;

    controller_state->dpad_up = !gpio_get(BUTTON_PINS[BTN_INDEX_UP]);
    controller_state->dpad_down = !gpio_get(BUTTON_PINS[BTN_INDEX_DOWN]);
    controller_state->dpad_left = !gpio_get(BUTTON_PINS[BTN_INDEX_LEFT]);
    controller_state->dpad_right = !gpio_get(BUTTON_PINS[BTN_INDEX_RIGHT]);

    adc_select_input(0);
    controller_state->left_stick_x = adc_read();
    adc_select_input(1);
    controller_state->left_stick_y = adc_read();
}

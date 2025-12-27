#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool button_a;
    bool button_b;
    bool button_x;
    bool button_y;

    bool dpad_up;
    bool dpad_down;
    bool dpad_left;
    bool dpad_right;
    
    // 12-bit precision
    uint16_t left_stick_x;
    uint16_t left_stick_y;

    uint16_t right_stick_x;
    uint16_t right_stick_y;
} controller_state_t;

void poll_right_inputs(controller_state_t *controller_state);
void poll_left_inputs(controller_state_t *controller_state);

#endif  // GAMEPAD_H

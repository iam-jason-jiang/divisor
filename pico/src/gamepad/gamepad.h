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
    
    int16_t left_stick_x;
    int16_t left_stick_y;

    int16_t right_stick_x;
    int16_t right_stick_y;
} controller_state_t;

void gamepad_init_hw(void);
void poll_right_inputs(controller_state_t *controller_state);
void poll_left_inputs(controller_state_t *controller_state);

#endif  // GAMEPAD_H

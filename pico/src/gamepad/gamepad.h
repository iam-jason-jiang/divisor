#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool button1;
    bool button2;
    bool button3;
    bool button4;

    bool button5;
    bool button6;
    bool button7;
    bool button8;
    
    // 12-bit precision
    uint16_t left_stick_x;
    uint16_t left_stick_y;

    uint16_t right_stick_x;
    uint16_t right_stick_y;
} controller_state_t;

void usb_task(controller_state_t *controller_state);

#endif  // GAMEPAD_H

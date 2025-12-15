#ifndef GAMEPAD_H
#define GAMEPAD_H

typedef struct {
    int button1;
    int button2;
    int button3;
    int button4;

    int button5;
    int button6;
    int button7;
    int button8;
    
    float left_stick_x;
    float left_stick_y;

    float right_stick_x;
    float right_stick_y;
} controller_state_t;

void usb_task(controller_state_t *controller_state);

#endif  // GAMEPAD_H

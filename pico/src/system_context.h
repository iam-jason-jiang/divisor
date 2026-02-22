#ifndef SYSTEM_CONTEXT_H
#define SYSTEM_CONTEXT_H

#include "gamepad.h"

typedef struct {
    controller_state_t gamepad;

    // TODO
    // uart_state_t       uart;
    // led_state_t        led;
} system_context_t;

#endif // SYSTEM_CONTEXT_H

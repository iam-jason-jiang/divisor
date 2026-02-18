#include "gamepad.h"
#include "bsp/board_api.h"

typedef struct {
    bool raw;
    uint32_t last_change_ms;
    bool stable;
} button_state_t;

button_state_t debounce_state[8];

void debounce(controller_state_t *state) {
    // we do this as we can just loop through the state rather the assgining each loop
    bool *curr_state[8] = {&state->button1, &state->button2, &state->button3, &state->button4, &state->button5, &state->button6, &state->button7, &state->button8};
    
    for (int i = 0; i < 8; i++) {
        bool curr_button = *curr_state[i];
        
        // just chekcing if the there  has been a change in current state button
        if (curr_button != debounce_state[i].raw) {
            debounce_state[i].raw = curr_button;
            debounce_state[i].last_change_ms = board_millis();
        }
        // If the last change has occured more than 5 ms ago then change change the scurrent stable state
        if ((board_millis() - debounce_state[i].last_change_ms) >= 5) {
            debounce_state[i].stable = debounce_state[i].raw;
        }
    
        *curr_state[i] = debounce_state[i].stable;
    }
}
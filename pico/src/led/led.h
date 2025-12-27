#ifndef LED_H
#define LED_H

#include <stdint.h>

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum {
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED = 1000,
    BLINK_SUSPENDED = 2500,
};

extern uint32_t blink_interval_ms;

int pico_led_init(void);
void led_blinking_task(void);

#endif  // LED_H

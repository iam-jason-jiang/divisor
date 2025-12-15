#include "led.h"

#include <stdbool.h>
#include <stdint.h>

#include "bsp/board_api.h"
#include "pico/stdlib.h"

#if PICO_CYW43_SUPPORT
#include "pico/cyw43_arch.h"
#endif

int pico_led_init(void) {
#if PICO_CYW43_SUPPORT
    if (cyw43_arch_init() != PICO_OK) {
        return PICO_ERROR_GENERIC;
    }
#elif defined(PICO_DEFAULT_LED_PIN)
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#endif
    return PICO_OK;
}

void pico_set_led(bool led_on) {
#if PICO_CYW43_SUPPORT
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#elif defined(PICO_DEFAULT_LED_PIN)
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#endif
}

void led_blinking_task(void) {
    static uint32_t start_ms = 0;
    static bool led_state = false;

    // blink is disabled
    if (!blink_interval_ms) return;

    // Blink every interval ms
    if (board_millis() - start_ms < blink_interval_ms)
        return;  // not enough time
    start_ms += blink_interval_ms;

    pico_set_led(led_state);
    led_state = 1 - led_state;  // toggle
}

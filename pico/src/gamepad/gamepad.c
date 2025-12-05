#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board_api.h"
#include "tusb.h"

#include "led.h"
#include "usb_callbacks.h"
#include "usb_descriptors.h"

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

/*
GAMEPAD_BUTTON_12 DPAD_UP
GAMEPAD_BUTTON_13 DPAD_DOWN
GAMEPAD_BUTTON_14 DPAD_LEFT
GAMEPAD_BUTTON_15 DPAD_RIGHT
*/

static void send_hid_report(uint32_t btn) {
  // skip if hid is not ready yet
  if (!tud_hid_ready())
    return;

  // use to avoid send multiple consecutive zero reports
  static bool has_gamepad_key = false;

  hid_gamepad_report_t report = {.x = 0,
                                 .y = 0,
                                 .z = 0,
                                 .rz = 0,
                                 .rx = 0,
                                 .ry = 0,
                                 .hat = 0,
                                 .buttons = 0};

  if (btn) {
    report.hat = GAMEPAD_HAT_DOWN;
    report.buttons |= GAMEPAD_BUTTON_13;
    report.buttons |= GAMEPAD_BUTTON_A;
    tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
    has_gamepad_key = true;
  } else {
    if (has_gamepad_key)
      tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
    has_gamepad_key = false;
  }
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc
// ..) tud_hid_report_complete_cb() is used to send the next report after
// previous one is complete
void hid_task(void) {
  // Poll every 10ms
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if (board_millis() - start_ms < interval_ms)
    return; // not enough time
  start_ms += interval_ms;

  uint32_t const btn = board_button_read();

  // Remote wakeup
  if (tud_suspended() && btn) {
    // Wake up host if we are in suspend mode
    // and REMOTE_WAKEUP feature is enabled by host
    tud_remote_wakeup();
  } else {
    // Send the 1st of report chain, the rest will be sent by
    // tud_hid_report_complete_cb()
    send_hid_report(btn);
  }
}

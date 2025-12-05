#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board_api.h"
#include "tusb.h"

#include "led.h"
#include "usb_callbacks.h"
#include "usb_descriptors.h"

#include "device/usbd_pvt.h"

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

    hid_gamepad_report_t report = {
        .x = 0,
        .y = 0,
        .z = 0,
        .rz = 0,
        .rx = 0,
        .ry = 0,
        .hat = 0,
        .buttons = 0
    };

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

typedef struct {
    uint8_t rid;
    uint8_t rsize;
    uint8_t digital_buttons_1;
    uint8_t digital_buttons_2;
    uint8_t lt;
    uint8_t rt;
    int16_t l_x;
    int16_t l_y;
    int16_t r_x;
    int16_t r_y;
    uint8_t reserved_1[6];
} xinput_report_t;

uint8_t endpoint_in=0;
uint8_t endpoint_out=0;
static volatile bool is_xinput_xfer_busy = false;

static void xinput_send(xinput_report_t const *report) {
    if (endpoint_in != 0) {
        // We must ensure the endpoint is free before transferring.
        if (usbd_edpt_claim(0, endpoint_in)) { // <-- Checks if endpoint is free AND claims it
            // Start transfer
            if (usbd_edpt_xfer(0, endpoint_in, (uint8_t *) report, 20)) {
                // Transfer successfully started. Now, the xinput_xfer_cb will be called on completion.
                // We keep the endpoint claimed until the transfer finishes.
                is_xinput_xfer_busy = true;
            } else {
                usbd_edpt_release(0, endpoint_in);
            }
        }
    }
}

static void send_xinput_report(uint32_t btn) {
    if (!tud_ready())
        return;

    static bool has_gamepad_key = false;

    xinput_report_t report = {
        .rid = 0,
        .rsize = 0,
        .digital_buttons_1 = 0,
        .digital_buttons_2 = 0,
        .lt = 0,
        .rt = 0,
        .l_x = 0,
        .l_y = 0,
        .r_x = 0,
        .r_y = 0,
        .reserved_1 = {0}
    };
    report.rsize = 20;

    if (btn) {
        report.digital_buttons_2 = 0x10; // A button
        xinput_send(&report);

        has_gamepad_key = true;
    } else {
        if (has_gamepad_key)
            xinput_send(&report);
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

    if (tud_suspended() && btn) {
        // Remote wakeup
        tud_remote_wakeup();
    } else {
        if (input_mode == 1) {
            send_xinput_report(btn);
        } else {
            send_hid_report(btn);
        }
    }
}

static void xinput_init(void) {

}

static void xinput_reset(uint8_t __unused rhport) {

}

static uint16_t xinput_open(uint8_t __unused rhport, tusb_desc_interface_t const *itf_desc, uint16_t max_len) {
    //+16 is for the unknown descriptor
    uint16_t const drv_len = sizeof(tusb_desc_interface_t) + itf_desc->bNumEndpoints*sizeof(tusb_desc_endpoint_t) + 16;
    TU_VERIFY(max_len >= drv_len, 0);

    uint8_t const * p_desc = tu_desc_next(itf_desc);
    uint8_t found_endpoints = 0;
    while ( (found_endpoints < itf_desc->bNumEndpoints) && (drv_len <= max_len)  )
    {
        tusb_desc_endpoint_t const * desc_ep = (tusb_desc_endpoint_t const *) p_desc;
        if ( TUSB_DESC_ENDPOINT == tu_desc_type(desc_ep) )
        {
            TU_ASSERT(usbd_edpt_open(rhport, desc_ep));

            if ( tu_edpt_dir(desc_ep->bEndpointAddress) == TUSB_DIR_IN )
            {
                endpoint_in = desc_ep->bEndpointAddress;
            }else
            {
                endpoint_out = desc_ep->bEndpointAddress;
            }
            found_endpoints += 1;
        }
        p_desc = tu_desc_next(p_desc);
    }
    return drv_len;
}

static bool xinput_control_xfer_cb(uint8_t __unused rhport, uint8_t stage, tusb_control_request_t const *request) {
    return true;
}

//callback after xfer_transfer
static bool xinput_xfer_cb(uint8_t __unused rhport, uint8_t __unused ep_addr, xfer_result_t __unused result, uint32_t __unused xferred_bytes) {
    is_xinput_xfer_busy = false;
    return true;
}


static usbd_class_driver_t const xinput_driver ={
#if CFG_TUSB_DEBUG >= 2
    .name = "XINPUT",
#endif
    .init             = xinput_init,
    .reset            = xinput_reset,
    .open             = xinput_open,
    .control_xfer_cb  = xinput_control_xfer_cb,
    .xfer_cb          = xinput_xfer_cb,
    .sof              = NULL
};

// Implement callback to add our custom driver
usbd_class_driver_t const *usbd_app_driver_get_cb(uint8_t *driver_count) {
    *driver_count = 1;
    return &xinput_driver;
}

#ifndef USB_CALLBACKS_H
#define USB_CALLBACKS_H

#include <stdbool.h>
#include <stdint.h>

// Required TinyUSB callbacks
void tud_mount_cb(void);
void tud_umount_cb(void);
void tud_suspend_cb(bool remote_wakeup_en);
void tud_resume_cb(void);

#endif  // USB_CALLBACKS_H

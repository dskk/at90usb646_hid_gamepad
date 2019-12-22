#pragma once
#include "util.h"

void init(enum controller_type ct); // initialize everything

#if defined(__AVR_AT90USB162__)
#define HW_CONFIG()
#define PLL_CONFIG() (PLLCSR = ((1<<PLLE)|(1<<PLLP0)))
#define USB_CONFIG() (USBCON = (1<<USBE))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
//#define START_BOOTLOADER() void (*start_bootloader) (void)=(void (*)(void))0x1800;(*start_bootloader)();
#elif defined(__AVR_ATmega32U4__)
#define HW_CONFIG() (UHWCON = 0x01)
#define PLL_CONFIG() (PLLCSR = 0x12)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
//#define START_BOOTLOADER() void (*start_bootloader) (void)=(void (*)(void))0x3800;(*start_bootloader)();
#elif defined(__AVR_AT90USB646__)
#define HW_CONFIG() (UHWCON = 0x81)
#define PLL_CONFIG() (PLLCSR = 0x1A)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
//#define START_BOOTLOADER() void (*start_bootloader) (void)=(void (*)(void))0x7800;(*start_bootloader)();
#elif defined(__AVR_AT90USB1286__)
#define HW_CONFIG() (UHWCON = 0x81)
#define PLL_CONFIG() (PLLCSR = 0x16)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
//#define START_BOOTLOADER() void (*start_bootloader) (void)=(void (*)(void))0xf000;(*start_bootloader)();
#endif

extern uint8_t is_ps3;
extern const uint8_t* ep_list;
extern const uint8_t* desc_list;
extern uint8_t sizeof_ep_list;
extern uint8_t sizeof_desc_list;
extern uint8_t sizeof_hid_report;
extern uint8_t hid_report[64];
extern void (*hid_report_update)(void);

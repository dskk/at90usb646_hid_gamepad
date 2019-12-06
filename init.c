#include "util.h"
#include "init.h"
#include "usb.h"
#include "hid_report.h"
#include "desc/IIDX_PS3.h"

//extern variables
uint8_t is_ps3;
const uint8_t* ep_list;
const uint8_t* desc_list;
uint8_t sizeof_ep_list;
uint8_t sizeof_desc_list;
uint8_t hid_report[64];
uint8_t sizeof_hid_report;

// initialize USB
void usb_init(enum controller_type ct) {
    switch(ct){
    case IIDX_PS3:
        is_ps3 = 1;
        ep_list = (const uint8_t *) ep_list_IIDX_PS3;
        desc_list = (const uint8_t *) desc_list_IIDX_PS3;
        sizeof_ep_list = 2;
        sizeof_desc_list = 8;
        sizeof_hid_report = 8;
        break;
    }
    HW_CONFIG();  // UHWCON = 0x81 USB device mode && enable the USB pad regulator
    USB_FREEZE(); // enable the USB controller && disable the clock inputs
    PLL_CONFIG(); // (Clock division factor)=8 (External XTAL)=16MHz enable PLL
    while (!(PLLCSR & (1<<PLOCK))); // wait for PLL lock (it takes about 100 ms)
    USB_CONFIG(); // start USB clock
    UDCON = 0; // clear bit0 - DETACH (Set to physically detach
               // the device by disconnecting internal pull-up on D+ or D-).
    usb_configuration = 0; // zero when we are not configured, non-zero when enumerated
    UDIEN = (1<<EORSTE)|(1<<SOFE); //enable the EORSTI interrupt and SOFI interrput
    // EORSTI - End Of Reset Interrupt flag.
    // Set by hardware when an “End Of Reset” has been detected by the USB controller.
    // SOFI - Start Of Frame Interrupt flag.
    // Set by hardware when an USB “Start Of Frame” PID (SOF) has been detected (every 1ms).
    sei();
}

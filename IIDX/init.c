#include <avr/eeprom.h>
#include "util.h"
#include "init.h"
#include "usb.h"
#include "modes/common.h"

//extern variables
const uint8_t* ep_list;
const uint8_t* desc_list;
const func_ptr_t* hid_report_send_func_list;
const func_ptr_t* hid_report_recv_func_list;
uint8_t ep_list_len;
uint8_t desc_list_len;
uint8_t hid_report_send_func_list_len;
uint8_t hid_report_recv_func_list_len;

// initialize USB
void usb_init(void) {
    uint8_t eep_val = eeprom_read_byte(0x00); // read EEPROM
    eeprom_busy_wait();
    switch(eep_val){
    case 1: //PS3
        ep_list = (const uint8_t *) ep_list_PS3;
        desc_list = (const uint8_t *) desc_list_PS3;
        hid_report_send_func_list = (const func_ptr_t *) hid_report_send_func_list_PS3;
        hid_report_recv_func_list = (const func_ptr_t *) hid_report_recv_func_list_PS3;
        ep_list_len = 2;
        desc_list_len = 8;
        hid_report_send_func_list_len = 1;
        hid_report_recv_func_list_len = 1;
        is_ps3 = 1;
        break;
    default:
        for(;;);
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

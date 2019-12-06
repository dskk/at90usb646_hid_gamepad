#include <avr/io.h>
#include <util/delay.h>
#include "init.h"
#include "util.h"
#include "usb.h"
#include "hid_report.h"

int main(void) {
    CLKPR = 0x80; //clock prescaler change enable
    CLKPR = 0; //set for 16MHz clock

    enum controller_type ct;

    ct=IIDX_PS3;

    // Initialize the USB, and then wait for the host to set configuration.
    usb_init(ct);
    while (!usb_configured()) /* wait */ ;

    // Wait an extra second for the PC's operating system to load drivers
    // and do whatever it does to actually be ready for input
    _delay_ms(1000);

    while (1) {
        hid_report_update(ct);
        usb_gamepad_send();
    }
}

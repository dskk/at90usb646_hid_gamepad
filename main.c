//http://git.slashdev.ca/ps3-teensy-hid/tree/src
#include <avr/io.h>
#include <util/delay.h>
#include "init.h"
#include "util.h"
#include "usb.h"

inline void hid_report_send(void) {
    uint8_t intr_state, i;
    if (usb_configuration==0) return; // if not configured
    intr_state = SREG; // take memo for restoring SREG later
    cli(); // this will affect SREG
    UENUM = 1; // select endpoint 1
    if (!(UEINTX & (1<<RWAL))){ // if not ready to transmit
        SREG = intr_state; // restore SREG ( = sei() )
        return;
    }
    for (i=0; i<sizeof_hid_report; i++) UEDATX = hid_report[i];
    UEINTX = 0x3A; // send data
    SREG = intr_state; // sei()
    return;
}

int main(void) {
   ACSR |= (1<<ACD); //ADC off
   asm volatile (
       "out %1, %0"      "\n\t"
       "out %1, %0"      "\n\t"
       :
       : "r" ((uint8_t) 1 << JTD), "i" (_SFR_IO_ADDR(MCUCR))
   );//JTAGに取られて使えないポートの開放のため。なんか二度書き込まないといけないとか特殊仕様だった
    DDRA=0;
    DDRB=0;
    DDRC=0;
    DDRD=0;
    DDRE=0;
    DDRF=0;
    PORTA=0xff;
    PORTB=0xff;
    PORTC=0xff;
    PORTD=0xff;
    PORTE=0xff;
    PORTF=0xff;
    //if(!(~PINA)) {START_BOOTLOADER();}
    CLKPR = 0x80; //clock prescaler change enable
    CLKPR = 0; //set for 16MHz clock

    // Initialize the USB, and then wait for the host to set configuration.
    init(IIDX_PS3);
    while (usb_configuration==0); // wait

    // Wait an extra second for the PC's operating system to load drivers
    // and do whatever it does to actually be ready for input
    _delay_ms(1000);

    while (1) {
        (*hid_report_update)();
        hid_report_send();
        //(*hid_report_send)();
        //(*hid_report_receive)();
    }
}

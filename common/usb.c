#include "common.h"
#include "usb.h"
#include "states_struct.h"
#include "modes.h"

//variables
volatile uint8_t usb_configuration = 0; // ->extern
volatile func_ptr_t usb_task_ptr; // -> extern
static volatile uint8_t gamepad_idle_config = 0;
static volatile uint8_t gamepad_protocol = 1;

void usb_init(void) {
    uint8_t eep_val = eeprom_read_byte(EEP_ADDR); // read EEPROM
    eeprom_busy_wait();
    switch(eep_val){
    #ifdef MODE_ENABLE_PS3
    case 1: //PS3
        ep_list = (const uint8_t *) ep_list_PS3;
        desc_list = (const uint8_t *) desc_list_PS3;
        ep_list_len = 2;
        desc_list_len = 7;
        is_ps3 = 1;
        usb_task_ptr=&usb_task_PS3;
        break;
    #endif
    #ifdef MODE_ENABLE_INFINITAS
    case 2: //INFINITAS
        ep_list = (const uint8_t *) ep_list_INFINITAS;
        desc_list = (const uint8_t *) desc_list_INFINITAS;
        ep_list_len = 2;
        desc_list_len = 7;
        is_ps3 = 0;
        usb_task_ptr=&usb_task_INFINITAS;
        break;
    #endif
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

// Misc functions to wait for ready and send/receive packets
static inline void usb_wait_in_ready(void) {
    while (!(UEINTX & (1<<TXINI))) ;
}
static inline void usb_send_in(void) {
    UEINTX = ~(1<<TXINI);
}
static inline void usb_wait_receive_out(void) {
    while (!(UEINTX & (1<<RXOUTI))) ;
}
static inline void usb_ack_out(void) {
    UEINTX = ~(1<<RXOUTI);
}
inline void configure_ep(int epnum){
    const uint8_t* ep = ep_list+epnum*sizeof(struct ep_list_struct); // ==&ep_list[0+offset]
    UENUM = epnum;
    UECONX = 1;
    UECFG0X = pgm_read_byte(ep); //==ep.data_transfer_type
    ep++;
    UECFG1X = pgm_read_byte(ep); //==(ep.size | ep.buffer_type)
    ep++;
}

ISR(USB_GEN_vect) { //handle EORSTI
    if (UDINT & (1<<EORSTI)) { // if End Of Reset Interrupt flag is set
        UDINT = 0; // clear interrupt flags
        configure_ep(0);
        UEIENX = (1<<RXSTPE); //Set to enable an endpoint interrupt (EPINTx) when RXSTPI is sent.
        // RXSTPI is Set by hardware to signal that the current bank contains a new valid SETUP packet.
        // RXSTPI is handled in USB_COM_vect
        usb_configuration = 0;
    }else{
        UDINT = 0; // clear interrupt flags
    }
}

// USB Endpoint Interrupt - endpoint 0 is handled here.  The
// other endpoints are manipulated by the user-callable
// functions, and the start-of-frame interrupt.
ISR(USB_COM_vect) {
    uint8_t i;
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;

    UENUM = 0;
    if (UEINTX & (1<<RXSTPI)) {
        bmRequestType = UEDATX;
        bRequest = UEDATX;
        wValue = UEDATX;
        wValue |= (UEDATX << 8);
        wIndex = UEDATX;
        wIndex |= (UEDATX << 8);
        wLength = UEDATX;
        wLength |= (UEDATX << 8);
        UEINTX = ~((1<<RXSTPI) | (1<<RXOUTI) | (1<<TXINI));
        if (REQUEST_TYPE(bmRequestType) == STANDARD_REQUEST) {
            if (bRequest == GET_DESCRIPTOR) {
                // descriptor to return is specified by its type(MSB) and index(LSB)
                const uint8_t* desc = desc_list;
                uint16_t desc_val;
                uint16_t desc_ind;
                const uint8_t* desc_addr;
                uint8_t desc_length;
                uint8_t n, len, isok = 0;
                for (i=0; i<desc_list_len; i++) {
                    desc_val = pgm_read_word(desc);
                    if (desc_val != wValue) {
                        desc += sizeof(struct desc_list_struct);
                        continue;
                    }
                    desc += 2;
                    desc_ind = pgm_read_word(desc);
                    if (desc_ind != wIndex) {
                        desc += sizeof(struct desc_list_struct)-2;
                        continue;
                    }
                    desc += 2;
                    desc_addr = (const uint8_t*)pgm_read_word(desc);
                    desc += 2;
                    desc_length = pgm_read_byte(desc);
                    isok = 1;
                    break;
                }
                if(!isok){
                    UECONX = (1<<STALLRQ) | (1<<EPEN);// stall
                    return;
                }
                len = (wLength < 256) ? wLength : 255;
                if (len > desc_length) len = desc_length;
                do {
                    // wait for host ready for IN packet
                    do {
                        i = UEINTX;
                    } while (!(i & ((1<<TXINI)|(1<<RXOUTI))));
                    if (i & (1<<RXOUTI)) return;    // abort
                    // send IN packet
                    n = len < 64 ? len : 64;
                    for (i = n; i>0; i--) {
                        UEDATX = pgm_read_byte(desc_addr++);
                    }
                    len -= n;
                    usb_send_in();
                } while (len || n == 64);
                return;
            }
            if (
                (bRequest == SET_CONFIGURATION) &&
                (REQUEST_DIRECTION(bmRequestType) == REQUEST_OUT) &&
                (REQUEST_RECIPIENT(bmRequestType) == RECIPIENT_DEVICE)
            ) {
                usb_configuration = wValue;
                usb_send_in();
                for (i=1; i<ep_list_len; i++) configure_ep(i);
                UERST = 0x1E;
                UERST = 0;
                return;
            }
            if (bRequest == SET_ADDRESS) {
                usb_send_in();
                usb_wait_in_ready();
                UDADDR = wValue | (1<<ADDEN);
                return;
            }
            if (
                (bRequest == GET_CONFIGURATION) &&
                (REQUEST_DIRECTION(bmRequestType) == REQUEST_IN) &&
                (REQUEST_RECIPIENT(bmRequestType) == RECIPIENT_DEVICE)
            ) {
                usb_wait_in_ready();
                UEDATX = usb_configuration;
                usb_send_in();
                return;
            }
            if (bRequest == GET_STATUS) {
                usb_wait_in_ready();
                UEDATX = 0;
                UEDATX = 0;
                usb_send_in();
                return;
            }
        }
        if (REQUEST_TYPE(bmRequestType) == CLASS_REQUEST) { // HID request
            if (REQUEST_DIRECTION(bmRequestType) == REQUEST_IN) {
                if (bRequest == HID_GET_REPORT) {
                    if(is_ps3) {
                        usb_wait_in_ready();
                        UEDATX = 0x21; // "PS3 magic init bytes"
                        UEDATX = 0x26;
                        UEDATX = 0x01;
                        UEDATX = 0x07;
                        UEDATX = 0x00;
                        UEDATX = 0x00;
                        UEDATX = 0x00;
                        UEDATX = 0x00;
                        usb_send_in();
                        return;
                    }
                }
                if (bRequest == HID_GET_IDLE) {
                    usb_wait_in_ready();
                    UEDATX = gamepad_idle_config;
                    usb_send_in();
                    return;
                }
                if (bRequest == HID_GET_PROTOCOL) {
                    usb_wait_in_ready();
                    UEDATX = gamepad_protocol;
                    usb_send_in();
                    return;
                }
            }
            if (REQUEST_DIRECTION(bmRequestType) == REQUEST_OUT) {
                if (bRequest == HID_SET_REPORT) {
                    usb_wait_receive_out();
                    usb_ack_out();
                    usb_send_in();
                    return;
                }
                if (bRequest == HID_SET_IDLE) {
                    gamepad_idle_config = MSB(wValue);
                    usb_send_in();
                    return;
                }
                if (bRequest == HID_SET_PROTOCOL) {
                    gamepad_protocol = wValue;
                    usb_send_in();
                    return;
                }
            }
        }
    }
    UECONX = (1<<STALLRQ) | (1<<EPEN); // stall
}

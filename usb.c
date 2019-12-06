#include "util.h"
#include "init.h"
#include "hid_report.h"
#include "usb.h"
#include "common.h"

/**************************************************************************
 *
 *  Variables - these are the only non-stack RAM usage
 *
 **************************************************************************/

// zero when we are not configured, non-zero when enumerated
volatile uint8_t usb_configuration = 0;

// return 0 if the USB is not configured, or the configuration
// number selected by the HOST
uint8_t usb_configured(void) { return usb_configuration; }

static uint8_t gamepad_idle_config = 0; 

// protocol setting from the host.  We use exactly the same report
// either way, so this variable only stores the setting since we
// are required to be able to report which setting is in use.
static uint8_t gamepad_protocol = 1;

/**************************************************************************
 *
 *  Public Functions - these are the API intended for the user
 *
 **************************************************************************/

inline void usb_gamepad_send(void) {
    uint8_t intr_state, i;

    if (!usb_configured()) return;
    intr_state = SREG;
    cli();
    UENUM = 0;
    if (!(UEINTX & (1<<RWAL))) return; // if not ready to transmit

    for (i=0; i<sizeof_hid_report; i++) {
        UEDATX = hid_report[i];
    }

    UEINTX = 0x3A; // send data
    SREG = intr_state; // sei()
    return;
}

/**************************************************************************
 *
 *  Private Functions - not intended for general user consumption....
 *
 **************************************************************************/

ISR(USB_GEN_vect) { //handle EORSTI
    uint8_t intbits;

    intbits = UDINT;
    UDINT = 0; // clear interrupt flags
    if (intbits & (1<<EORSTI)) { // if End Of Reset Interrupt flag is set
        UENUM = 0; // select endpoint 0 (control)
        UECONX = 1; // bit0 : Set to enable the endpoint according to the device configuration.
        UECFG0X = EP_TYPE_CONTROL; //Configure the endpoint direction and the endpoint type
        UECFG1X = EP_SIZE(64) | EP_SINGLE_BUFFER; // Configure the endpoint size and the bank parametrization. Allocation and reorganization of the memory is made on-the-fly
        UEIENX = (1<<RXSTPE); //Set to enable an endpoint interrupt (EPINTx) when RXSTPI is sent.
        // RXSTPI is Set by hardware to signal that the current bank contains a new valid SETUP packet.
        // RXSTPI is handled in USB_COM_vect
        usb_configuration = 0;
    }
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

// USB Endpoint Interrupt - endpoint 0 is handled here.  The
// other endpoints are manipulated by the user-callable
// functions, and the start-of-frame interrupt.
ISR(USB_COM_vect) {
    uint8_t i, intbits;
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;

    UENUM = 0;
    intbits = UEINTX;
    if (intbits & (1<<RXSTPI)) {
        bmRequestType = UEDATX;
        bRequest = UEDATX;
        wValue = UEDATX;
        wValue |= (UEDATX << 8);
        wIndex = UEDATX;
        wIndex |= (UEDATX << 8);
        wLength = UEDATX;
        wLength |= (UEDATX << 8);
        UEINTX = ~((1<<RXSTPI) | (1<<RXOUTI) | (1<<TXINI));
        if (REQUEST_TYPE(bmRequestType)==0b00) {
            if (bRequest == GET_DESCRIPTOR) {
                // descriptor to return is specified by its type(MSB) and index(LSB)
                const uint8_t* desc = desc_list;
                uint16_t desc_val;
                uint16_t desc_ind;
                const uint8_t* desc_addr;
                uint8_t desc_length;
                uint8_t n, len, isok = 0;
                for (i=0; i<sizeof_desc_list; i++) {
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
                    for (i = n; i; i--) {
                        UEDATX = pgm_read_byte(desc_addr++);
                    }
                    len -= n;
                    usb_send_in();
                } while (len || n == 64);
                return;
            }
            if (bRequest == SET_CONFIGURATION && bmRequestType == 0) {
                usb_configuration = wValue;
                usb_send_in();
                const uint8_t* ep = ep_list;
                for (i=1; i<=sizeof_ep_list; i++) {
                    UENUM = i;
                    UECONX = 1;
                    UECFG0X = pgm_read_byte(ep); //ep.data_transfer_type
                    ep++;
                    UECFG1X = pgm_read_byte(ep); //ep.size | ep.buffer_type
                    ep++;
                }
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
            if (bRequest == GET_CONFIGURATION && bmRequestType == 0x80) {
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
        } else if (REQUEST_TYPE(bmRequestType)==0b01) { // HID request
            if (bmRequestType == 0xA1) { // in
                if (bRequest == HID_GET_REPORT && is_ps3) {
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
            if (bmRequestType == 0x21) { //out
                if (bRequest == HID_SET_REPORT) {
                    usb_wait_receive_out();
                    usb_ack_out();
                    usb_send_in();
                    return;
                }
                if (bRequest == HID_SET_IDLE) {
                    gamepad_idle_config = (wValue >> 8);
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
    UECONX = (1<<STALLRQ) | (1<<EPEN);// stall
}

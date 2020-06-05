#include "common.h"
#include "usb.h"
#include "modes.h"

#define VID_INFINITAS              0x1ccf
#define PID_INFINITAS              0x8048
#define STR_PRODUCT_INFINITAS      L"IIDX INFINITAS controller"
#define DEVICE_DESC_SIZE_INFINITAS 18
#define CONFIG_DESC_SIZE_INFINITAS 34
#define REPORT_DESC_SIZE_INFINITAS 50
#define NUM_EP_INFINITAS           1

const struct str_desc_struct str_desc_product_INFINITAS PROGMEM = {
    sizeof(STR_PRODUCT_INFINITAS),
    3,
    STR_PRODUCT_INFINITAS
};

const uint8_t device_desc_INFINITAS[] PROGMEM = {
    18,                // bLength
    1,                 // bDescriptorType
    0x00, 0x02,        // bcdUSB
    0,                 // bDeviceClass
    0,                 // bDeviceSubClass
    0,                 // bDeviceProtocol
    64,                // bMaxPacketSize0
    LSB(VID_INFINITAS), // idVendor[0]
    MSB(VID_INFINITAS), // idVendor[1]
    LSB(PID_INFINITAS), // idProduct[0]
    MSB(PID_INFINITAS), // idProduct[1]
    0x00, 0x10,        // bcdDevice (=version number)
    1,                 // iManufacturer
    2,                 // iProduct
    0,                 // iSerialNumber
    1                  // bNumConfigurations (this source does not support composite device)
};

const uint8_t config_desc_INFINITAS[] PROGMEM = {
    // configuration descriptor
    9,                              // bLength;
    2,                              // bDescriptorType;
    LSB(CONFIG_DESC_SIZE_INFINITAS),
    MSB(CONFIG_DESC_SIZE_INFINITAS), // wTotalLength
    1,                              // bNumInterfaces
    1,                              // bConfigurationValue
    0,                              // iConfiguration
    0x80,                           // bmAttributes
    50,                             // bMaxPower
    // interface descriptor
    9,                              // bLength
    4,                              // bDescriptorType
    0,                              // bInterfaceNumber
    0,                              // bAlternateSetting
    NUM_EP_INFINITAS,               // bNumEndpoints (is 2 if the device has INTR_IN)
    0x03,                           // bInterfaceClass (0x03 = HID)
    0x00,                           // bInterfaceSubClass (0x00 = No Boot)
    0x00,                           // bInterfaceProtocol (0x00 = No Protocol)
    0,                              // iInterface
    // HID interface descriptor
    9,                              // bLength
    0x21,                           // bDescriptorType
    0x11, 0x01,                     // bcdHID
    0,                              // bCountryCode
    1,                              // bNumDescriptors
    0x22,                           // bDescriptorType
    LSB(REPORT_DESC_SIZE_INFINITAS),
    MSB(REPORT_DESC_SIZE_INFINITAS), // wDescriptorLength
    // endpoint descriptor
    7,                              // bLength
    5,                              // bDescriptorType
    1 | 0x80,                       // bEndpointAddress (|0x80=in)
    0x03,                           // bmAttributes (0x03=intr)
    64, 0,                          // wMaxPacketSize
    4                               // bInterval
    // add some descriptors if needed
};

const uint8_t report_desc_INFINITAS[] PROGMEM = {
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x04,        // Usage (Joystick)
    0xA1, 0x01,        // Collection (Application)
    0x09, 0x01,        //   Usage (Pointer)
    0xA1, 0x00,        //   Collection (Physical)
    0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
    0x09, 0x30,        //     Usage (X)
    0x09, 0x31,        //     Usage (Y)
    0x15, 0x81,        //     Logical Minimum (-127)
    0x25, 0x7F,        //     Logical Maximum (127)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x02,        //     Report Count (2)
    0x81, 0x02,        //     Input (Data,Var,Abs)
    0x05, 0x09,        //     Usage Page (Button)
    0x19, 0x01,        //     Usage Minimum (0x01)
    0x29, 0x10,        //     Usage Maximum (0x10)
    0x15, 0x00,        //     Logical Minimum (0)
    0x25, 0x01,        //     Logical Maximum (1)
    0x75, 0x01,        //     Report Size (1)
    0x95, 0x10,        //     Report Count (16)
    0x81, 0x02,        //     Input (Data,Var,Abs)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x03,        //     Input (Const,Var,Abs)
    0xC0,              //   End Collection
    0xC0               // End Collection
};

const struct ep_list_struct ep_list_INFINITAS[] PROGMEM = {
    {EP_TYPE_CONTROL,   EP_SIZE(64)|EP_SINGLE_BUFFER},
    {EP_TYPE_INTERRUPT_IN, EP_SIZE(64)|EP_DOUBLE_BUFFER}
};

const struct desc_list_struct desc_list_INFINITAS[] PROGMEM = {
    {0x0100, 0x0000, (const uint8_t*)device_desc_INFINITAS,                            DEVICE_DESC_SIZE_INFINITAS    },
    {0x0200, 0x0000, (const uint8_t*)config_desc_INFINITAS,                            CONFIG_DESC_SIZE_INFINITAS    },
    {0x2100, 0x0000, (const uint8_t*)config_desc_INFINITAS+DEVICE_DESC_SIZE_INFINITAS, 9                             },
    {0x2200, 0x0000, (const uint8_t*)report_desc_INFINITAS,                            REPORT_DESC_SIZE_INFINITAS    },
    {0x0300, 0x0000, (const uint8_t*)&str_desc_langID,                                 4                             },
    {0x0301, 0x0409, (const uint8_t*)&str_desc_manufacturer,                           sizeof(STR_MANUFACTURER)      },
    {0x0302, 0x0409, (const uint8_t*)&str_desc_product_INFINITAS,                      sizeof(STR_PRODUCT_INFINITAS) }
};

void hid_report_send_INFINITAS(uint8_t byte1, uint8_t byte3, uint8_t byte4) { //dir=in ep=1
    uint8_t intr_state;
    if (usb_configuration==0) return; // if not configured
    intr_state = SREG; // take memo for restoring SREG later
    cli(); // this will affect SREG
    UENUM = 1; // select endpoint 1
    if (!(UEINTX & (1<<RWAL))){ // if not ready to transmit
        SREG = intr_state; // restore SREG ( = sei() )
        return;
    }
    UEDATX = byte1; //signed char X_AXIS
    UEDATX = 0;     //signed char Y_AXIS
    UEDATX = byte3; //button1-8
    UEDATX = byte4; //button9-16
    UEDATX = 0;     //constant 0
    UEINTX = 0x3A;  // send data
    SREG = intr_state; // sei()
    return;
}

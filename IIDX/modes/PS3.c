#include "common.h"
#include "util.h"
#include "init.h"
#include "usb.h"
#include "states_struct.h"

const struct str_desc_struct str_desc_product_PS3 PROGMEM = {
    sizeof(STR_PRODUCT_PS3),
    3,
    STR_PRODUCT_PS3
};

const uint8_t device_desc_PS3[] PROGMEM = {
    18,                // bLength
    1,                 // bDescriptorType
    0x00, 0x02,        // bcdUSB
    0,                 // bDeviceClass
    0,                 // bDeviceSubClass
    0,                 // bDeviceProtocol
    64,                // bMaxPacketSize0
    LSB(VID_PS3), // idVendor[0]
    MSB(VID_PS3), // idVendor[1]
    LSB(PID_PS3), // idProduct[0]
    MSB(PID_PS3), // idProduct[1]
    0x00, 0x10,        // bcdDevice (=version number)
    1,                 // iManufacturer
    2,                 // iProduct
    0,                 // iSerialNumber
    1                  // bNumConfigurations (this source does not support composite device)
};

const uint8_t config_desc_PS3[] PROGMEM = {
    // configuration descriptor
    9,                              // bLength;
    2,                              // bDescriptorType;
    LSB(CONFIG_DESC_SIZE_PS3),
    MSB(CONFIG_DESC_SIZE_PS3), // wTotalLength
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
    NUM_EP_PS3,                // bNumEndpoints (is 2 if the device has INTR_IN)
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
    LSB(REPORT_DESC_SIZE_PS3),
    MSB(REPORT_DESC_SIZE_PS3), // wDescriptorLength
    // endpoint descriptor
    7,                              // bLength
    5,                              // bDescriptorType
    2,                              // bEndpointAddress (|0x80=in)
    0x03,                           // bmAttributes (0x03=intr)
    64, 0,                          // wMaxPacketSize
    10,                              // bInterval
    // endpoint descriptor
    7,                              // bLength
    5,                              // bDescriptorType
    1 | 0x80,                       // bEndpointAddress (|0x80=in)
    0x03,                           // bmAttributes (0x03=intr)
    64, 0,                          // wMaxPacketSize
    10                              // bInterval
    // add some descriptors if needed
};

const uint8_t report_desc_PS3[] PROGMEM = {
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x05,        // Usage (Game Pad)
    0xA1, 0x01,        // Collection (Application)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x35, 0x00,        //   Physical Minimum (0)
    0x45, 0x01,        //   Physical Maximum (1)
    0x75, 0x01,        //   Report Size (1)
    0x95, 0x0D,        //   Report Count (13)
    0x05, 0x09,        //   Usage Page (Button)
    0x19, 0x01,        //   Usage Minimum (0x01)
    0x29, 0x0D,        //   Usage Maximum (0x0D)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x03,        //   Report Count (3)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
    0x25, 0x07,        //   Logical Maximum (7)
    0x46, 0x3B, 0x01,  //   Physical Maximum (315)
    0x75, 0x04,        //   Report Size (4)
    0x95, 0x01,        //   Report Count (1)
    0x65, 0x14,        //   Unit (System: English Rotation, Length: Centimeter)
    0x09, 0x39,        //   Usage (Hat switch)
    0x81, 0x42,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
    0x65, 0x00,        //   Unit (None)
    0x95, 0x01,        //   Report Count (1)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x46, 0xFF, 0x00,  //   Physical Maximum (255)
    0x09, 0x30,        //   Usage (X)
    0x09, 0x31,        //   Usage (Y)
    0x09, 0x32,        //   Usage (Z)
    0x09, 0x35,        //   Usage (Rz)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x04,        //   Report Count (4)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
    0x09, 0x20,        //   Usage (0x20)
    0x09, 0x21,        //   Usage (0x21)
    0x09, 0x22,        //   Usage (0x22)
    0x09, 0x23,        //   Usage (0x23)
    0x09, 0x24,        //   Usage (0x24)
    0x09, 0x25,        //   Usage (0x25)
    0x09, 0x26,        //   Usage (0x26)
    0x09, 0x27,        //   Usage (0x27)
    0x09, 0x28,        //   Usage (0x28)
    0x09, 0x29,        //   Usage (0x29)
    0x09, 0x2A,        //   Usage (0x2A)
    0x09, 0x2B,        //   Usage (0x2B)
    0x95, 0x0C,        //   Report Count (12)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x0A, 0x21, 0x26,  //   Usage (0x2621)
    0x95, 0x08,        //   Report Count (8)
    0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x0A, 0x21, 0x26,  //   Usage (0x2621)
    0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x26, 0xFF, 0x03,  //   Logical Maximum (1023)
    0x46, 0xFF, 0x03,  //   Physical Maximum (1023)
    0x09, 0x2C,        //   Usage (0x2C)
    0x09, 0x2D,        //   Usage (0x2D)
    0x09, 0x2E,        //   Usage (0x2E)
    0x09, 0x2F,        //   Usage (0x2F)
    0x75, 0x10,        //   Report Size (16)
    0x95, 0x04,        //   Report Count (4)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0               // End Collection
};

const struct ep_list_struct ep_list_PS3[] PROGMEM = {
    {EP_TYPE_CONTROL,   EP_SIZE(64)|EP_SINGLE_BUFFER},
    {EP_TYPE_INTERRUPT_IN, EP_SIZE(64)|EP_DOUBLE_BUFFER}
};

const struct desc_list_struct desc_list_PS3[] PROGMEM = {
    {0x0100, 0x0000, (const uint8_t*)device_desc_PS3,                      DEVICE_DESC_SIZE_PS3    },
    {0x0200, 0x0000, (const uint8_t*)config_desc_PS3,                      CONFIG_DESC_SIZE_PS3    },
    {0x2100, 0x0000, (const uint8_t*)config_desc_PS3+DEVICE_DESC_SIZE_PS3, 9                       },
    {0x2200, 0x0000, (const uint8_t*)report_desc_PS3,                      REPORT_DESC_SIZE_PS3    },
    {0x0300, 0x0000, (const uint8_t*)&str_desc_langID,                     4                       },
    {0x0301, 0x0409, (const uint8_t*)&str_desc_manufacturer,               sizeof(STR_MANUFACTURER)},
    {0x0302, 0x0409, (const uint8_t*)&str_desc_product_PS3,                sizeof(STR_PRODUCT_PS3) },
    {0x0303, 0x0409, (const uint8_t*)&str_desc_serial,                     sizeof(STR_SERIAL)      }
};

/*
reorderL=[]
for i in range(64):
    res=0
    for j in range(6):
        if i&(1<<j):
            x=16 if j%2 else 1
            res+=(1<<(j//2))*x
    reorderL.append(res)
*/
const uint8_t reorderL[64] = { // xx654321 -> x642x531
    0, 1, 16, 17, 2, 3, 18, 19, 32, 33, 48, 49, 34, 35, 50, 51,
    4, 5, 20, 21, 6, 7, 22, 23, 36, 37, 52, 53, 38, 39, 54, 55,
    64, 65, 80, 81, 66, 67, 82, 83, 96, 97, 112, 113, 98, 99,
    114, 115, 68, 69, 84, 85, 70, 71, 86, 87, 100, 101, 116, 117,
    102, 103, 118, 119
};

const uint8_t reorderH[16] = { // xxxx4321 -> xxx4xx12
    0, 2, 1, 3, 0, 2, 1, 3, 16, 18, 17, 19, 16, 18, 17, 19
};

void hid_report_send_1_PS3(void) { //dir=in ep=1
    uint8_t intr_state, i;
    if (usb_configuration==0) return; // if not configured
    intr_state = SREG; // take memo for restoring SREG later
    cli(); // this will affect SREG
    UENUM = 1; // select endpoint 1
    if (!(UEINTX & (1<<RWAL))){ // if not ready to transmit
        SREG = intr_state; // restore SREG ( = sei() )
        return;
    }
    UEDATX = reorderL[states_str.button_state&0b111111];
    UEDATX = reorderH[(states_str.button_state>>7)&0b1111];
    if(states_str.button_state&0b1000000000){ // right pressed
        if(states_str.scratch_state==1) UEDATX = 0x01; //up pressed
        else if(states_str.scratch_state==2) UEDATX = 0x03; //down pressed
        else UEDATX = 0x02;
    }else if(states_str.button_state&0b1000000){ //left pressed
        if(states_str.scratch_state==1) UEDATX = 0x07; //up pressed
        else if(states_str.scratch_state==2) UEDATX = 0x05; //down pressed
        else UEDATX = 0x06;
    }else{
        if(states_str.scratch_state==1) UEDATX = 0x00; //up pressed
        else if(states_str.scratch_state==2) UEDATX = 0x04; //down pressed
        else UEDATX = 0x0f;
    }
    UEDATX = 0x80;
    UEDATX = 0x80;
    UEDATX = 0x80;
    UEDATX = 0x80;
    UEDATX = 0;
    UEDATX = 0;
    UEDATX = 0;
    UEDATX = 0;
    UEDATX = 0;
    UEDATX = 0;
    UEDATX = 0;
    UEDATX = 0;
    UEDATX = 0;
    UEDATX = 0;
    UEDATX = 0;
    UEDATX = 0;
    UEDATX = 0;
    UEDATX = 2;
    UEDATX = 0;
    UEDATX = 2;
    UEDATX = 0;
    UEDATX = 2;
    UEDATX = 0;
    UEDATX = 2;
    UEINTX = 0x3A; // send data
    SREG = intr_state; // sei()
    return;
}

void hid_report_recv_2_PS3(void) { //dir=out ep=2
    uint8_t intr_state;
    if (usb_configuration==0) return; // if not configured
    intr_state = SREG; // take memo for restoring SREG later
    cli(); // this will affect SREG
    UENUM = 2; // select endpoint 1
    if (!(UEINTX & (1<<RWAL))){ // if not ready to transmit
        SREG = intr_state; // restore SREG ( = sei() )
        return;
    }
    //for (i=0; i<sizeof_hid_report; i++) UEDATX = hid_report[i];
    UEINTX &= !(1<<FIFOCON); // clear FIFOCON to tell all the data is read
    SREG = intr_state; // restore SREG ( = sei() )
    return;
}

const func_ptr_t hid_report_send_func_list_PS3[] = {
    hid_report_send_1_PS3
};

const func_ptr_t hid_report_recv_func_list_PS3[] = {
    hid_report_recv_2_PS3
};

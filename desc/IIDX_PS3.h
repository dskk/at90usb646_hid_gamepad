#pragma once
#include "common.h"
#include "util.h"

#define VID_IIDX_PS3              0x034c
#define PID_IIDX_PS3              0x0368
#define STR_PRODUCT_IIDX_PS3      L"PS3 IIDX controller"
#define DEVICE_DESC_SIZE_IIDX_PS3 18
#define CONFIG_DESC_SIZE_IIDX_PS3 34
#define REPORT_DESC_SIZE_IIDX_PS3 112
#define NUM_EP_IIDX_PS3           1

const struct str_desc_struct str_desc_product_IIDX_PS3 PROGMEM = {
    sizeof(STR_PRODUCT_IIDX_PS3),
    3,
    STR_PRODUCT_IIDX_PS3
};

const uint8_t device_desc_IIDX_PS3[] PROGMEM = {
    18,                // bLength
    1,                 // bDescriptorType
    0x10, 0x01,        // bcdUSB
    0,                 // bDeviceClass
    0,                 // bDeviceSubClass
    0,                 // bDeviceProtocol
    64,                // bMaxPacketSize0
    LSB(VID_IIDX_PS3), // idVendor[0]
    MSB(VID_IIDX_PS3), // idVendor[1]
    LSB(PID_IIDX_PS3), // idProduct[0]
    MSB(PID_IIDX_PS3), // idProduct[1]
    0x00, 0x01,        // bcdDevice (=version number)
    1,                 // iManufacturer
    2,                 // iProduct
    3,                 // iSerialNumber
    1                  // bNumConfigurations (this source does not support composite device)
};

const uint8_t config_desc_IIDX_PS3[] PROGMEM = {
    // configuration descriptor
    9,                              // bLength;
    2,                              // bDescriptorType;
    LSB(CONFIG_DESC_SIZE_IIDX_PS3),
    MSB(CONFIG_DESC_SIZE_IIDX_PS3), // wTotalLength
    1,                              // bNumInterfaces
    1,                              // bConfigurationValue
    0,                              // iConfiguration
    0x80,                           // bmAttributes
    250,                            // bMaxPower
    // interface descriptor
    9,                              // bLength
    4,                              // bDescriptorType
    0,                              // bInterfaceNumber
    0,                              // bAlternateSetting
    NUM_EP_IIDX_PS3,                // bNumEndpoints (is 2 if the device has INTR_IN)
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
    LSB(REPORT_DESC_SIZE_IIDX_PS3),
    MSB(REPORT_DESC_SIZE_IIDX_PS3), // wDescriptorLength
    // endpoint descriptor
    7,                              // bLength
    5,                              // bDescriptorType
    1 | 0x80,                       // bEndpointAddress (|0x80=in)
    0x03,                           // bmAttributes (0x03=intr)
    64, 0,                          // wMaxPacketSize
    10                              // bInterval
    // add some descriptors if needed
};

const uint8_t report_desc_IIDX_PS3[] PROGMEM = {
    0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,        // USAGE (Gamepad)
    0xa1, 0x01,        // COLLECTION (Application)
    0x15, 0x00,        //   LOGICAL_MINIMUM (0)
    0x25, 0x01,        //   LOGICAL_MAXIMUM (1)
    0x35, 0x00,        //   PHYSICAL_MINIMUM (0)
    0x45, 0x01,        //   PHYSICAL_MAXIMUM (1)
    0x75, 0x01,        //   REPORT_SIZE (1)
    0x95, 0x0d,        //   REPORT_COUNT (13)
    0x05, 0x09,        //   USAGE_PAGE (Button)
    0x19, 0x01,        //   USAGE_MINIMUM (Button 1)
    0x29, 0x0d,        //   USAGE_MAXIMUM (Button 13)
    0x81, 0x02,        //   INPUT (Data,Var,Abs)
    0x95, 0x03,        //   REPORT_COUNT (3)
    0x81, 0x01,        //   INPUT (Cnst,Ary,Abs)
    0x05, 0x01,        //   USAGE_PAGE (Generic Desktop)
    0x25, 0x07,        //   LOGICAL_MAXIMUM (7)
    0x46, 0x3b, 0x01,  //   PHYSICAL_MAXIMUM (315)
    0x75, 0x04,        //   REPORT_SIZE (4)
    0x95, 0x01,        //   REPORT_COUNT (1)
    0x65, 0x14,        //   UNIT (Eng Rot:Angular Pos)
    0x09, 0x39,        //   USAGE (Hat switch)
    0x81, 0x42,        //   INPUT (Data,Var,Abs,Null)
    0x65, 0x00,        //   UNIT (None)
    0x95, 0x01,        //   REPORT_COUNT (1)
    0x81, 0x01,        //   INPUT (Cnst,Ary,Abs)
    0x26, 0xff, 0x00,  //   LOGICAL_MAXIMUM (255)
    0x46, 0xff, 0x00,  //   PHYSICAL_MAXIMUM (255)
    0x09, 0x30,        //   USAGE (X)
    0x09, 0x31,        //   USAGE (Y)
    0x09, 0x32,        //   USAGE (Z)
    0x09, 0x35,        //   USAGE (Rz)
    0x75, 0x08,        //   REPORT_SIZE (8)
    0x95, 0x04,        //   REPORT_COUNT (4)
    0x81, 0x02,        //   INPUT (Data,Var,Abs)
    0x06, 0x00, 0xff,  //   USAGE_PAGE (Vendor Specific)
    0x09, 0x20,        //   Unknown
    0x09, 0x21,        //   Unknown
    0x09, 0x22,        //   Unknown
    0x09, 0x23,        //   Unknown
    0x09, 0x24,        //   Unknown
    0x09, 0x25,        //   Unknown
    0x09, 0x26,        //   Unknown
    0x09, 0x27,        //   Unknown
    0x09, 0x28,        //   Unknown
    0x09, 0x29,        //   Unknown
    0x09, 0x2a,        //   Unknown
    0x09, 0x2b,        //   Unknown
    0x95, 0x0c,        //   REPORT_COUNT (12)
    0x81, 0x02,        //   INPUT (Data,Var,Abs)
    0x0a, 0x21, 0x26,  //   Unknown
    0x95, 0x08,        //   REPORT_COUNT (8)
    0xb1, 0x02,        //   FEATURE (Data,Var,Abs)
    0xc0               // END_COLLECTION
};

const struct ep_list_struct ep_list_IIDX_PS3[] PROGMEM = {
    {EP_TYPE_CONTROL,   EP_SIZE(64)|EP_SINGLE_BUFFER},
    {EP_TYPE_INTERRUPT_IN, EP_SIZE(64)|EP_DOUBLE_BUFFER}
};

const struct desc_list_struct desc_list_IIDX_PS3[] PROGMEM = {
    {0x0100, 0x0000, (const uint8_t*)device_desc_IIDX_PS3,       DEVICE_DESC_SIZE_IIDX_PS3   },
    {0x0200, 0x0000, (const uint8_t*)config_desc_IIDX_PS3,       CONFIG_DESC_SIZE_IIDX_PS3   },
    {0x2100, 0x0000, (const uint8_t*)config_desc_IIDX_PS3+18,    9                           },
    {0x2200, 0x0000, (const uint8_t*)report_desc_IIDX_PS3,       REPORT_DESC_SIZE_IIDX_PS3   },
    {0x0300, 0x0000, (const uint8_t*)&str_desc_langID,           4                           },
    {0x0301, 0x0409, (const uint8_t*)&str_desc_manufacturer,     sizeof(STR_MANUFACTURER)    },
    {0x0302, 0x0409, (const uint8_t*)&str_desc_product_IIDX_PS3, sizeof(STR_PRODUCT_IIDX_PS3)},
    {0x0302, 0x0409, (const uint8_t*)&str_desc_serial,           sizeof(STR_SERIAL)          }
};

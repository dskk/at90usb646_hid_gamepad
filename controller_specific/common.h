// *memo: descriptor type*
// 01h device desc
// 02h configuration desc
// 03h string desc
// 04h interface desc
// 05h endpoint desc (not necessary for EP0)
// 21h HID desc
// 22h HID report desc
#pragma once
#include "util.h"

#define STR_MANUFACTURER  L"Cialis"
#define STR_SERIAL        L"191206_000"

struct ep_list_struct {
    uint8_t data_transfer_type;
    uint8_t ep_size_buffer_type; // =ep_size|buffer_type
    // data_transfer_type and buffer_type is defined in init.h
};
struct desc_list_struct {
    uint16_t       wValue; // MSB=descriptor type (see memo) LSB=descriptor index
    uint16_t       wIndex;
    const uint8_t* addr;
    uint8_t        length;
};
struct str_desc_struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    int16_t wString[];
};

const struct str_desc_struct str_desc_langID;
const struct str_desc_struct str_desc_manufacturer;
const struct str_desc_struct str_desc_serial;

// IIDX_PS3
#define VID_IIDX_PS3              0x034c
#define PID_IIDX_PS3              0x0368
#define STR_PRODUCT_IIDX_PS3      L"PS3 IIDX controller"
#define DEVICE_DESC_SIZE_IIDX_PS3 18
#define CONFIG_DESC_SIZE_IIDX_PS3 34
#define REPORT_DESC_SIZE_IIDX_PS3 112
#define NUM_EP_IIDX_PS3           1
const struct str_desc_struct str_desc_product_IIDX_PS3;
const uint8_t device_desc_IIDX_PS3[];
const uint8_t config_desc_IIDX_PS3[];
const uint8_t report_desc_IIDX_PS3[];
const struct ep_list_struct ep_list_IIDX_PS3[];
const struct desc_list_struct desc_list_IIDX_PS3[];
void hid_report_update_IIDX_PS3(void);
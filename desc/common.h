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

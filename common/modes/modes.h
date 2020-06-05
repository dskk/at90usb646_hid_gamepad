// *memo: descriptor type*
// 01h device desc
// 02h configuration desc
// 03h string desc
// 04h interface desc
// 05h endpoint desc (not necessary for EP0)
// 21h HID desc
// 22h HID report desc
#pragma once
#include "common.h"

#define STR_MANUFACTURER  L"Cialis"

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
//const struct str_desc_struct str_desc_serial;

// PS3
const struct str_desc_struct str_desc_product_PS3;
const uint8_t device_desc_PS3[];
const uint8_t config_desc_PS3[];
const uint8_t report_desc_PS3[];
const struct ep_list_struct ep_list_PS3[];
const struct desc_list_struct desc_list_PS3[];
void hid_report_send_PS3(uint8_t byte1, uint8_t byte2, uint8_t byte3);
void hid_report_recv_PS3(void);
// INFINITAS
const struct str_desc_struct str_desc_product_INFINITAS;
const uint8_t device_desc_INFINITAS[];
const uint8_t config_desc_INFINITAS[];
const uint8_t report_desc_INFINITAS[];
const struct ep_list_struct ep_list_INFINITAS[];
const struct desc_list_struct desc_list_INFINITAS[];
void hid_report_send_INFINITAS(uint8_t byte1, uint8_t byte3, uint8_t byte4);
// SDVXCLOUD
const struct str_desc_struct str_desc_product_SDVXCLOUD;
const uint8_t device_desc_SDVXCLOUD[];
const uint8_t config_desc_SDVXCLOUD[];
const uint8_t report_desc_SDVXCLOUD[];
const struct ep_list_struct ep_list_SDVXCLOUD[];
const struct desc_list_struct desc_list_SDVXCLOUD[];
void hid_report_send_SDVXCLOUD(uint8_t byte1, uint8_t byte2, uint8_t byte3);
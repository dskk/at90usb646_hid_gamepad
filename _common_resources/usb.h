#pragma once

// standard control endpoint request types
#define GET_STATUS          0
#define CLEAR_FEATURE       1
#define SET_FEATURE         3
#define SET_ADDRESS         5
#define GET_DESCRIPTOR      6
#define GET_CONFIGURATION   8
#define SET_CONFIGURATION   9
#define GET_INTERFACE       10
#define SET_INTERFACE       11
// HID (human interface device)
#define HID_GET_REPORT      1
#define HID_GET_IDLE        2
#define HID_GET_PROTOCOL    3
#define HID_SET_REPORT      9
#define HID_SET_IDLE        10
#define HID_SET_PROTOCOL    11
// bmRequestType masks and constants
#define REQUEST_TYPE(n)       (n & 0b01100000)
#define STANDARD_REQUEST           0b00000000
#define CLASS_REQUEST              0b00100000
#define REQUEST_DIRECTION(n)  (n & 0b10000000)
#define REQUEST_OUT                0b00000000
#define REQUEST_IN                 0b10000000
#define REQUEST_RECIPIENT(n)  (n & 0b00011111)
#define RECIPIENT_DEVICE           0b00000000
#define RECIPIENT_INTERFACE        0b00000001
#define RECIPIENT_ENDPOINT         0b00000010
#define RECIPIENT_OTHERS           0b00000011

extern volatile uint8_t usb_configuration;
extern uint8_t is_ps3;

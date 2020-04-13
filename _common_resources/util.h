#pragma once
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define EP_TYPE_CONTROL         0x00
#define EP_TYPE_INTERRUPT_IN    0xC1
#define EP_TYPE_INTERRUPT_OUT   0xC0

#define EP_SINGLE_BUFFER        0x02
#define EP_DOUBLE_BUFFER        0x06

#define EP_SIZE(s) ((s) == 64 ? 0x30 :\
                   ((s) == 32 ? 0x20 :\
                   ((s) == 16 ? 0x10 :\
                                0x00)))

#define LSB(n) (n & 255)
#define MSB(n) ((n >> 8) & 255)

#define EEP_ADDR                0x00

typedef void (*func_ptr_t)(void);
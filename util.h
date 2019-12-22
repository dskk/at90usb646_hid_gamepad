#pragma once
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

enum  controller_type { IIDX_PS3, IIDX_PC_KONAMI, IIDX_PC_NORMAL, SDVX_PC_KONAMI, SDVX_PC_NORMAL, POPN_PS3, POPN_PC_NORMAL };

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

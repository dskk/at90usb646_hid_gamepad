#pragma once
#include "common.h"
#include "modes.h"

#define MODE_ENABLE_PS3
#define MODE_ENABLE_INFINITAS

struct states_str_t{
    uint16_t  button_state; //pressed=1 else=0
    uint8_t   button_is_locked[16];
    int32_t   button_locked_time[16];
    uint8_t   scratch_state; //neutral=0 up=1 down=2
    uint16_t  scratch_position;
    uint8_t   scratch_is_locked;
    int32_t   scratch_locked_time;
    int32_t   scratch_kept_time;
};

void usb_task_PS3(void);
void usb_task_INFINITAS(void);

extern struct states_str_t states_str;
#pragma once
#include "common.h"
#include "modes.h"

#define MODE_ENABLE_SDVXCLOUD

struct states_str_t{
    uint16_t  button_state; //pressed=1 else=0
    uint8_t   button_is_locked[16];
    int32_t   button_locked_time[16];
    int8_t    vol_l_position;
    int8_t    vol_r_position;
};

void usb_task_SDVXCLOUD(void);

extern struct states_str_t states_str;
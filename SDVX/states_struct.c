#include "states_struct.h"

void usb_task_SDVXCLOUD(void){
    uint8_t byte3=(states_str.button_state<<1)&127;
    byte3|=(states_str.button_state>>6)&0b1;
    hid_report_send_SDVXCLOUD((uint8_t)states_str.vol_l_position, (uint8_t)states_str.vol_r_position, byte3);
}

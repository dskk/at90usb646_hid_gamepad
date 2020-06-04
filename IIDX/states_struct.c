#include "states_struct.h"

const uint8_t reorderL[64] = { // xx654321 -> x642x531
    0, 1, 16, 17, 2, 3, 18, 19, 32, 33, 48, 49, 34, 35, 50, 51,
    4, 5, 20, 21, 6, 7, 22, 23, 36, 37, 52, 53, 38, 39, 54, 55,
    64, 65, 80, 81, 66, 67, 82, 83, 96, 97, 112, 113, 98, 99,
    114, 115, 68, 69, 84, 85, 70, 71, 86, 87, 100, 101, 116, 117,
    102, 103, 118, 119
};

const uint8_t reorderH[16] = { // xxxx4321 -> xxx4xx12
    0, 2, 1, 3, 0, 2, 1, 3, 16, 18, 17, 19, 16, 18, 17, 19
};

void usb_task_PS3(void){
    uint8_t byte1 = reorderL[states_str.button_state&0b111111];
    uint8_t byte2 = reorderH[(states_str.button_state>>7)&0b1111];
    uint8_t byte3 = 0x0f;
    if(states_str.button_state&0b1000000000){ // right pressed
        if(states_str.scratch_state==1) byte3 = 0x01; //up pressed
        else if(states_str.scratch_state==2) byte3 = 0x03; //down pressed
        else byte3 = 0x02;
    }else if(states_str.button_state&0b1000000){ //left pressed
        if(states_str.scratch_state==1) byte3 = 0x07; //up pressed
        else if(states_str.scratch_state==2) byte3 = 0x05; //down pressed
        else byte3 = 0x06;
    }else{
        if(states_str.scratch_state==1) byte3 = 0x00; //up pressed
        else if(states_str.scratch_state==2) byte3 = 0x04; //down pressed
        //else byte3 = 0x0f; //default
    }
    hid_report_send_PS3(byte1, byte2, byte3);
    hid_report_recv_PS3();
}

void usb_task_INFINITAS(void){
    int8_t  byte1 = (states_str.scratch_position&255)-127;
    uint8_t byte3 = states_str.button_state&127;
    uint8_t byte4 = states_str.button_state>>7;
    hid_report_send_INFINITAS((uint8_t)byte1, byte3, byte4);
}
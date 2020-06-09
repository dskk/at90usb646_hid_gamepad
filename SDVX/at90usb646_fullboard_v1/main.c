#include <util/delay.h>
#include "common.h"
#include "usb.h"
#include "modes.h"
#include "states_struct.h"

/* board specific */
#if defined(__AVR_AT90USB162__)
func_ptr_t start_bootloader=(func_ptr_t)0x1800;
#elif defined(__AVR_ATmega32U4__)
func_ptr_t start_bootloader=(funcp_tr_t)0x3800;
#elif defined(__AVR_AT90USB646__)
func_ptr_t start_bootloader=(func_ptr_t)0x7800;
#elif defined(__AVR_AT90USB1286__)
func_ptr_t start_bootloader=(func_ptr_t)0xf000;
#endif

#define NUM_BUTTON_IN  7
#define NUM_BUTTON_OUT 7

struct states_str_t states_str;
const uint8_t photo_state[] = { 0, 3, 1, 2 }; // IIDXと逆回りなことに注意
uint8_t last_photo_state_l, last_photo_state_r;

//NC(押されると1,普段は0): A7,A5,A3,A1,F7,F5,F3,F1
//NO(押されると0,普段は1): A6,A4,A2,A0,F6,F4,F2,F0
//LED+:                    B7,B6,B5,B4,B3,B2,B1,B0
//いずれも左からButton1...Button8。Button8は未使用。
//Button1から順にA,B,C,D,L,R,START
volatile uint8_t* in_pin_nc[NUM_BUTTON_IN]={
    &PINA, //A
    &PINA, //B
    &PINA, //C
    &PINA, //D
    &PINF, //L
    &PINF, //R
    &PINF  //START
};
uint8_t in_bit_nc[NUM_BUTTON_IN]={
    7,     //A
    5,     //B
    3,     //C
    1,     //D
    7,     //L
    5,     //R
    3      //START
};
volatile uint8_t* in_pin_no[NUM_BUTTON_IN]={
    &PINA, //A
    &PINA, //B
    &PINA, //C
    &PINA, //D
    &PINF, //L
    &PINF, //R
    &PINF  //START
};
uint8_t in_bit_no[NUM_BUTTON_IN]={
    6,     //A
    4,     //B
    2,     //C
    0,     //D
    6,     //L
    4,     //R
    2      //START
};
volatile uint8_t* out_pin[NUM_BUTTON_OUT]={
    &PINB, //A
    &PINB, //B
    &PINB, //C
    &PINB, //D
    &PINB, //L
    &PINB, //R
    &PINB  //START
};
uint8_t out_bit[NUM_BUTTON_OUT]={
    7,      //A
    6,      //B
    5,      //C
    4,      //D
    3,      //L
    2,      //R
    1       //START
};

inline void io_task(void){
    uint16_t mask=1;
    for(uint8_t i=0; i<NUM_BUTTON_IN; i++, mask<<=1){
        if(bit_is_clear(*in_pin_no[i],in_bit_no[i])){ //NOがGNDに落ちている:ボタンが押されている
            states_str.button_state|=mask;
            *out_pin[i]|=(1<<out_bit[i]);
        }
        if(bit_is_clear(*in_pin_nc[i],in_bit_nc[i])){ //NCがGNDに落ちている:ボタンが押されていない
            states_str.button_state&=~mask;
            *out_pin[i]&=~(1<<out_bit[i]);
        }
    }
    if(photo_state[(PINC>>4)&0b11]==((last_photo_state_l+1)%4)){
        states_str.vol_l_position++;
    }
    else if(photo_state[(PINC>>4)&0b11]==((last_photo_state_l+3)%4)){
        states_str.vol_l_position--;
    }
    if(photo_state[(PINC>>6)&0b11]==((last_photo_state_r+1)%4)){
        states_str.vol_r_position++;
    }
    else if(photo_state[(PINC>>6)&0b11]==((last_photo_state_r+3)%4)){
        states_str.vol_r_position--;
    }
    last_photo_state_l=photo_state[(PINC>>4)&0b11];
    last_photo_state_r=photo_state[(PINC>>6)&0b11];
}

inline void io_init(void){
    ACSR |= (1<<ACD); //ADC off
    asm volatile (
        "out %1, %0"      "\n\t"
        "out %1, %0"      "\n\t"
        :
        : "r" ((uint8_t) 1 << JTD), "i" (_SFR_IO_ADDR(MCUCR))
    );// Disable JTAG so as to use PORTF bit4-7 as I/O pins.
    CLKPR = 0x80; //clock prescaler change enable
    CLKPR = 0; //set for 16MHz clock
    DDRA=0;
    DDRB=0xFF;
    DDRC=0;
    DDRD=0;
    DDRE=0;
    DDRF=0;
    PORTA=0xFF;
    PORTB=0;
    PORTC=0xFF;
    PORTD=0xFF;
    PORTE=0xFF; //PS機能なし時
    PORTF=0xFF;
    last_photo_state_l=(PINC>>4)&0b11;
    last_photo_state_r=(PINC>>6);

    if(bit_is_clear(*in_pin_no[4],in_bit_no[4]) && bit_is_clear(*in_pin_no[5],in_bit_no[5])) start_bootloader();
    //Timer3とmode change は省略。DAOコンのmain.c参照。
    states_str.button_state=0;
    states_str.vol_l_position=0;
    states_str.vol_r_position=0;
    last_photo_state_l=photo_state[(PINC>>4)&0b11];
    last_photo_state_r=photo_state[(PINC>>6)&0b11];
//    eeprom_write_byte(EEP_ADDR, 16);
//    eeprom_busy_wait();
    return;
}
/* board specific */

int main(void) {
    io_init();
    usb_init();
    while (usb_configuration==0); // wait for the host to set configuration.
    // Wait an extra second for the PC's operating system to load drivers
    // and do whatever it does to actually be ready for input
    _delay_ms(1000);
    while (1) {
        io_task();
        (*usb_task_ptr)();
    }
}

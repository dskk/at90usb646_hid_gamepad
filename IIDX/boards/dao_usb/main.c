//http://git.slashdev.ca/ps3-teensy-hid/tree/src
#include <avr/io.h>
#include <util/delay.h>
#include "init.h"
#include "util.h"
#include "usb.h"
#include "states_struct.h"
#include "common.h"

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

struct states_str_t states_str;
const uint16_t button_lock_duration=20000; //20000/2us=10ms
const uint16_t scratch_lock_duration=20000; //20000/2us=10ms
const uint16_t scratch_keep_duration=18750; //18750*16us=300ms
const uint8_t photo_state[] = { 0, 1, 3, 2 };
int8_t last_photo_state;

inline void io_init(void){
    ACSR |= (1<<ACD); //ADC off
    asm volatile (
        "out %1, %0"      "\n\t"
        "out %1, %0"      "\n\t"
        :
        : "r" ((uint8_t) 1 << JTD), "i" (_SFR_IO_ADDR(MCUCR))
    );// Disable JTAG so as to use PORTF bit4-7 as I/O pins.
    DDRA=0;
    DDRB=0;
    DDRC=0xFF;
    DDRD=0xFF;
    DDRE=0;
    DDRF=0;
    PORTA=0xff;
    PORTB=0xff;
    PORTC=0;
    PORTD=0;
    PORTE=0xff;
    PORTF=0xff;
    if(bit_is_clear(PINF,5) && bit_is_clear(PINF,6)) start_bootloader();
    TCCR1A=0; //タイマ1の波形出力設定。波形出力なし
    TCCR1B=2; //通常モード、1/8分周=2MHzでタイマ1(16bitタイマ)スタート。65536/2us=32.768msでオーバーフローする。
    TCCR3A=0; //タイマ3の波形出力設定。波形出力なし
    TCCR3B=4; //通常モード、1/256分周=1/16MHzでタイマ3(16bitタイマ)スタート。65536*16us=1048.576msでオーバーフローする。
    states_str.button_state=0;
    states_str.scratch_state=0;
    states_str.scratch_position=0;
    for(uint8_t i=0; i<16; i++){
        states_str.button_is_locked[i]=0;
        states_str.scratch_is_locked=0;
    }
    last_photo_state=photo_state[PINE>>6];
    return;
}

volatile uint8_t* in_pin[11]={
    &PINA, //1
    &PINA, //2
    &PINA, //3
    &PINA, //4
    &PINA, //5
    &PINA, //6
    &PINA, //7
    &PINF, //8(E1)
    &PINF, //9(E2)
    &PINF, //10(E3)
    &PINA  //11(E4)
};
uint8_t in_bit[11]={
    7, //1
    6, //2
    5, //3
    4, //4
    3, //5
    2, //6
    1, //7
    5, //8(E1)
    6, //9(E2)
    7, //10(E3)
    0  //11(E4)
};
volatile uint8_t* out_pin[11]={
    &PINC, //1
    &PINC, //2
    &PINC, //3
    &PINC, //4
    &PINC, //5
    &PINC, //6
    &PINC, //7
    &PIND, //8(E1)
    &PIND, //9(E2)
    &PIND, //10(E3)
    &PINC  //11(E4)
};
uint8_t out_bit[11]={
    7, //1
    6, //2
    5, //3
    4, //4
    3, //5
    2, //6
    1, //7
    5, //8(E1)
    6, //9(E2)
    7, //10(E3)
    0  //11(E4)
};

inline void update(void){
    uint16_t mask=1;
    for(uint8_t i=0; i<11; i++, mask<<=1){
        if(bit_is_clear(*in_pin[i],in_bit[i])) *out_pin[i]|=(1<<out_bit[i]); //ボタンが押されている:点灯
        else *out_pin[i]&=~(1<<out_bit[i]); //ボタンが押されていない:消灯
        if(states_str.button_is_locked[i]){ //チャタリング対策でlockされているボタンについては、lockが解除できるか調べる
            if(TCNT1<states_str.button_locked_time[i]) states_str.button_locked_time[i]-=65536; //タイマーが次の周に入っているときは(共通で用いるタイマーの値を変えられないので)locked_timeを前周の値(<0)にする
            if(TCNT1-states_str.button_locked_time[i]>button_lock_duration) states_str.button_is_locked[i]=0; //button_lock_durationより長く経過したらlock解除;
        }
        if(!states_str.button_is_locked[i]){ //直上でlockが解除されるかもしれないのでelseではなくif not
            if((states_str.button_state&mask)==0){ //stateのbitが0=押されていない
                if(bit_is_clear(*in_pin[i],in_bit[i])){ //ボタンが押されている
                    states_str.button_state|=mask; //ビット立てる
                    states_str.button_locked_time[i]=TCNT1;
                    states_str.button_is_locked[i]=1;
                }
            }else{//stateのbitが1=押されている
                if(bit_is_set(*in_pin[i],in_bit[i])){
                    states_str.button_state&=~mask; //ビット下げる
                    states_str.button_locked_time[i]=TCNT1;
                    states_str.button_is_locked[i]=1;
                }
            }
        }
    }
    if(states_str.scratch_is_locked){
        if(TCNT1<states_str.scratch_locked_time) states_str.scratch_locked_time-=65536;
        if(TCNT1-states_str.scratch_locked_time>scratch_lock_duration) states_str.scratch_is_locked=0;
    }
    if(states_str.scratch_state!=0){
        if(TCNT3<states_str.scratch_kept_time) states_str.scratch_kept_time-=65536;
        if(TCNT3-states_str.scratch_kept_time>scratch_keep_duration) states_str.scratch_state=0;
    }
    if(photo_state[PINE>>6]==((last_photo_state+1)%4)){
        states_str.scratch_position++;
        if(!states_str.scratch_is_locked){
            if(states_str.scratch_state!=1){
                states_str.scratch_state=1;
                states_str.scratch_is_locked=1;
                states_str.scratch_locked_time=TCNT1;
            }
            states_str.scratch_kept_time=TCNT3;
        }
    }
    else if(photo_state[PINE>>6]==((last_photo_state+3)%4)){
        states_str.scratch_position--;
        if(!states_str.scratch_is_locked){
            if(states_str.scratch_state!=2){
                states_str.scratch_state=2;
                states_str.scratch_is_locked=1;
                states_str.scratch_locked_time=TCNT1;
            }
            states_str.scratch_kept_time=TCNT3;
        }
    }
    last_photo_state=photo_state[PINE>>6];
}
/* board specific */

int main(void) {
    io_init();
    CLKPR = 0x80; //clock prescaler change enable
    CLKPR = 0; //set for 16MHz clock

    // Initialize the USB, and then wait for the host to set configuration.
    //if(!(~PINA)) {START_BOOTLOADER();}
    init();
    while (usb_configuration==0); // wait
    // Wait an extra second for the PC's operating system to load drivers
    // and do whatever it does to actually be ready for input
    _delay_ms(1000);
    while (1) {
        update();
        for(uint8_t i=0;i<hid_report_send_func_list_len;i++) hid_report_send_func_list[i]();
        for(uint8_t i=0;i<hid_report_recv_func_list_len;i++) hid_report_recv_func_list[i]();
    }
}

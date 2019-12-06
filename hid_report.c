#include "util.h"
#include "hid_report.h"
#include "init.h"
/*
#define PORT1        PORTB
#define PORT1_PINS    (BTN_CROSS | BTN_SQUARE | BTN_TRIANGLE | \
            BTN_CIRCLE | BTN_R1 | BTN_R2 | BTN_L1 | BTN_L2)
#define PORT2        PORTD
#define PORT2_PINS    (JOYSTICK_LEFT | JOYSTICK_RIGHT | JOYSTICK_UP | \
            JOYSTICK_DOWN | BTN_START | BTN_SELECT | BTN_PS)

//init
PORT1=PORT1_PINS;
PORT2=PORT2_PINS;

#define BTN_CROSS    (1 << 0)
#define BTN_SQUARE    (1 << 1)
#define BTN_TRIANGLE    (1 << 2)
#define BTN_CIRCLE    (1 << 3)

#define BTN_R1    (1 << 4)
#define BTN_R2    (1 << 5)
#define BTN_L1    (1 << 6)
#define BTN_L2    (1 << 7)

#define BTN_CROSS_ON        (PINB & BTN_CROSS) == 0
#define BTN_SQUARE_ON    (PINB & BTN_SQUARE) == 0
#define BTN_TRIANGLE_ON    (PINB & BTN_TRIANGLE) == 0
#define BTN_CIRCLE_ON    (PINB & BTN_CIRCLE) == 0

#define BTN_R1_ON    (PINB & BTN_R1) == 0
#define BTN_R2_ON    (PINB & BTN_R2) == 0
#define BTN_L1_ON    (PINB & BTN_L1) == 0
#define BTN_L2_ON    (PINB & BTN_L2) == 0

#define JOYSTICK_RIGHT (1 << 0)
#define JOYSTICK_LEFT  (1 << 1)
#define JOYSTICK_UP    (1 << 2)
#define JOYSTICK_DOWN  (1 << 3)

#define BTN_START  (1 << 4)
#define BTN_SELECT (1 << 5)
#define BTN_PS     (1 << 7)

#define JOYSTICK_RIGHT_ON    (PIND & JOYSTICK_RIGHT) == 0
#define JOYSTICK_LEFT_ON    (PIND & JOYSTICK_LEFT) == 0
#define JOYSTICK_UP_ON    (PIND & JOYSTICK_UP) == 0
#define JOYSTICK_DOWN_ON    (PIND & JOYSTICK_DOWN) == 0

#define BTN_START_ON        (PIND & BTN_START) == 0
#define BTN_SELECT_ON    (PIND & BTN_SELECT) == 0
#define BTN_PS_ON        (PIND & BTN_PS) == 0
*/

void hid_report_update(enum controller_type ct){
    switch(ct){
    case IIDX_PS3:
        hid_report_update_IIDX_PS3();
        break;
    }
}

void hid_report_update_IIDX_PS3(void){
    return;
}

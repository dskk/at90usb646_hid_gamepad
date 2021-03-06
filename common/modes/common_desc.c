#include "common.h"
#include "modes.h"

//string descriptor 0 = language ID
const struct str_desc_struct str_desc_langID PROGMEM = {
    4,
    3,
    {0x0409}
};
//string descriptor 1 = manufacturer
const struct str_desc_struct str_desc_manufacturer PROGMEM = {
    sizeof(STR_MANUFACTURER),
    3,
    STR_MANUFACTURER
};
/* 使わない
#define STR_SERIAL        L"191206_000"
//string descriptor 2 = serial number
const struct str_desc_struct str_desc_serial PROGMEM = {
    sizeof(STR_SERIAL),
    3,
    STR_SERIAL
};
*/
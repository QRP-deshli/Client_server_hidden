// Client-server api              //
// Long-term shared secret        //
// Version 0.5.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 19.11.2024                     //
// Nikita Kuropatkin              //

/* 
This header file contains the long-term shared secret for authenticating both sides
(it can be changed, but must be updated on both sides).
*/
#ifndef SECRET_H
#define SECRET_H
#include <stdint.h>

//Long-term shared secret for both sides
static uint8_t secured_key[32] = {
        0x51,0x1F,0x68,0x66,
        0x2D,0xBB,0x48,0xAA,
        0x17,0xC1,0x9F,0xA7,
        0xEE,0xCD,0x8D,0xC5,
        0xAD,0x92,0x4A,0xB2,
        0x7E,0x83,0x7B,0xBE,
        0xDE,0x71,0x23,0x30,
        0xEF,0xF8,0x1D,0xF2
};

#endif
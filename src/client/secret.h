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
0xFE,0xDC,0x3C,0x3B,0xD1,0xFA,0xDA,0xD2,0xB0,0xAB,0x24,0x72,0xE2,0x3A,0x94,0x9C,0x70,0x8D,0x86,0xEB,0x10,0xEC,0x16,0xD1,0xDA,0xF0,0xA3,0xBB,0x9E,0x1A,0x1C,0x12};
#endif
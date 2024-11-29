// Client-server api              //
// Long-term shared secret(client)//
// Version 0.6                    //
// Bachelor`s work project        //
// Technical University of Kosice //
// 28.11.2024                     //
// Nikita Kuropatkin              //

/* 
This header file contains the long-term shared secret secured by PIN 
and will be used by client for authentication of other side.
*/
#ifndef SECRET_H
#define SECRET_H
#include <stdint.h>

//Long-term shared secret secured by PIN
static uint8_t secured_key[32] = {
        0xC0,0xE8,0x39,0xF5,
        0xE2,0xAD,0xB4,0xCC,
        0x7D,0x46,0x0D,0x20,
        0x66,0x68,0xFA,0x37,
        0x72,0x01,0xC1,0xE3,
        0x4F,0xD9,0xEC,0x0E,
        0xB3,0xD7,0x89,0xF9,
        0xBB,0xFA,0x0C,0x6D
};

#endif
// Client-server api              //
// Long-term shared secret(server)//
// Version 0.6.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 05.12.2024                     //
// Nikita Kuropatkin              //

/* 
This header file contains the long-term shared secret(server) for
authentication of other side.
It can be changed, but must be updated on both sides
(Client should use pin_changer.exe to secure plain key).
*/
#ifndef SECRET_H
#define SECRET_H
#include <stdint.h>

//Long-term shared secret for both authenticating sides
const uint8_t key_original[32] = {
	0xA3,0x80,0x58,0x34,
	0x7C,0x46,0x6C,0x2D,
	0xDC,0x3F,0x6B,0xDE,
	0xB1,0xCB,0x08,0x02,
	0xD4,0xC8,0xEE,0x41,
	0x9A,0xF7,0x83,0x29,
	0x11,0x83,0x6B,0x0B,
	0x03,0x82,0x97,0xF7
};

#endif
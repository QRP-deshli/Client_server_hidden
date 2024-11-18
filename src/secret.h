// Client-server api              //
// Additional functions           //
// Version 0.5                    //
// Bachelor`s work project        //
// Technical University of Kosice //
// 10.11.2024                     //
// Nikita Kuropatkin              //

/* 
This header file declares additional functions
for a client-server application. Function definitions
are in addition.c. 
*/
#ifndef SECRET_H
#define SECRET_H
//Long-term shared secret for both sides
uint8_t key_original[32] = {
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
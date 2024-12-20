// Client-server api              //
// Random number generator        //
// Version 0.7.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 20.12.2024                     //
// Nikita Kuropatkin              //

/* 
This header file declares CSPRNG(random number generator) function
for a client-server application. Function definition
are in random.c. 
*/
#ifndef RANDOM_H
#define RANDOM_H
#include <stdint.h>

//////////////////////////////////////////
/// Random Numbers Generator ///
//////////////////////////////////////////
/*
The purpose of this function is to generate random bytes  
and fill the provided array with them.  
It takes the following parameters:  
- `buffer` - an array to be filled with random bytes.  
- `size` - the number of random bytes to generate and store in the buffer.  
This function works on both Windows (WIN) and Linux (LIN).
*/
void random_num(uint8_t * number, const int size);
//////////////////////////////////////////
//////////////////////////////////////////

#endif
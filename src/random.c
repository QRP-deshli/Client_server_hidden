// Client-server api              //
// Random number generator        //
// Version 0.7.0                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 16.12.2024                     //
// Nikita Kuropatkin              //

#include "include/error.h" //All errors defined + function proto
#include "include/random.h"
/*Windows Libs*/
#ifdef _WIN32
    /*Macro for rand_s(Windows OS)*/
    #define _CRT_RAND_S 
    #include <stdlib.h>
/*Linux Libs*/
#else 
    #include <bsd/stdlib.h>
#endif

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
void random_num(uint8_t * number, const int size) {
 #ifdef _WIN32
 unsigned int temp;
 for (int i = 0;i<size;i++) {
    if (rand_s(&temp) != 0) {
        exit_with_error(ERROR_GENERATING_RANDOM, "Number generation failed");
    }
    number[i] = (uint8_t)temp;
 }  
 #else
    arc4random_buf(number, size);
    if (number == NULL) {
        exit_with_error(ERROR_GENERATING_RANDOM, "Number generation failed");
    }
 #endif
}

// Client-server api              //
// Random number generator        //
// Version 0.6.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 05.12.2024                     //
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
/// Random numbers generator ///
//////////////////////////////////////////
/*
This function purpose is to generate random bits 
and fill received array with them.
This function works for Win and Lin.
*/
void random_num(uint8_t * number, const int size){
 #ifdef _WIN32
 unsigned int temp;
 for(int i = 0;i<size;i++){
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

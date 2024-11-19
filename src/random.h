// Client-server api              //
// Random number generator        //
// Version 0.5.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 10.11.2024                     //
// Nikita Kuropatkin              //

/* 
This header file declares CSPRNG(random number generator) function
for a client-server application. Function definition
are in random.c. 
*/
#ifndef RANDOM_H
#define RANDOM_H
#include <stdint.h>
#ifdef _WIN32
    #define _CRT_RAND_S //rand_s
    #include <stdlib.h>
//Linux Libs
#else
    #include <bsd/stdlib.h>
#endif
#include "error.h"//all errors defined + function proto

//////////////////////////////////////////
/// Random numbers generator ///
//////////////////////////////////////////
/*
This function purpose is to generate random bits and fill received array with them.
This function works for Win and Lin.
*/
void random_num(uint8_t * number, int size);
//////////////////////////////////////////
//////////////////////////////////////////

#endif
// Client-server api              //
// Random number generator        //
// Version 0.5.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 19.11.2024                     //
// Nikita Kuropatkin              //

/* 
This header file declares CSPRNG(random number generator) function
for a client-server application. Function definition
are in random.c. 
*/
#ifndef RANDOM_H
#define RANDOM_H
#include <stdint.h>
#ifdef _WIN32 //Windows Libs
    #include <stdlib.h>
#else //Linux Libs
    #include <bsd/stdlib.h>
#endif

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
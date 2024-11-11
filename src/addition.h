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
#ifndef ADDITION_H
#define ADDITION_H
#include <stdio.h>
#include <string.h>

#define EXIT "exit" // stop word, if someone use it in conversation, it will end

////////////////////////
/// Clearing input   ///
////////////////////////
/*This function purpose is to clear stdin after 
entering oversized message(stdin overflow)*/
void clear ();
////////////////////////
////////////////////////

///////////////////////////
/// Exit on stop-word   ///
///////////////////////////
/*This function purpose is to stop program
after detecting stop-word in message side 
is just a modifier for a message, that will
tell who ended communication*/
int exiting (char *side, char *msg);
///////////////////////////
///////////////////////////

#endif
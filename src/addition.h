// Client-server api              //
// Additional functions           //
// Version 0.5.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 19.11.2024                     //
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
#include <stdlib.h>
#include <error.h>
#define EXIT "exit" // stop word, if someone use it in conversation, it will end

////////////////////////
/// Clearing input   ///
////////////////////////
/*This function purpose is to clear stdin after 
entering oversized message(stdin overflow)*/
void clear (void);
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

///////////////////////////
/// Checking input IP   ///
///////////////////////////
/*This function purpose is to check ,whether
user entered valid IP-address*/
void ip_check (char *ip);
///////////////////////////
///////////////////////////

////////////////////////////////
/// Printing help for user   ///
////////////////////////////////
/*This function purpose is to print basic info 
for user and then end the program.
The parameter(int side) is a switch to print specific 
info for a specific side(server and client),basically it 
defines what side called this function.
Other parameters represent default values of port, message size and IP*/
void help_print (int side, int port, char *ip, int max);
////////////////////////////////
////////////////////////////////

#endif
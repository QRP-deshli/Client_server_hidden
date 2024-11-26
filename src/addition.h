// Client-server api              //
// Additional functions           //
// Version 0.5.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 19.11.2024                     //
// Nikita Kuropatkin              //

/* 
This header file declares additional functions, 
like printing help, ip validation, etc. for a 
client-server application. Function bodies
are in addition.c. 
*/
#ifndef ADDITION_H
#define ADDITION_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
after detecting stop-word in message,
variable side is just a modifier for a message, that will
tell who ended communication. You can configure stop-word, 
by changing macro EXIT.
Returns value 1 if the word was used, after that
program(server or client) proceeds in normal
closing communication session
*/
int exiting (char *side, char *msg);
///////////////////////////
///////////////////////////

///////////////////////////
/// Checking input IP   ///
///////////////////////////
/*This function purpose is to check, whether
user entered valid IP-address(should be in formated right). 
If inputed IP isn`t valid(isn`t formated right) - program exits
Right format examples:
# Dotted decimal form: 192.168.0.1
# Each octet MAX value is 255: 255.255.255.255 -- "maximum" ip address
*/
void ip_check (char *ip);
///////////////////////////
///////////////////////////

////////////////////////////////
/// Printing help for user   ///
////////////////////////////////
/*This function purpose is to print basic info 
for user and then end the program.
The parameter(int side) is a switch to print specific 
info for a specific side(server and client), basically it 
defines what side called this function.
Other parameters represent default values of port, message size and IP*/
void help_print (int side, int port, char *ip, int max);
////////////////////////////////
////////////////////////////////

#endif
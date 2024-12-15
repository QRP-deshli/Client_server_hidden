// Client-server api              //
// Additional functions           //
// Version 0.6.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 05.12.2024                     //
// Nikita Kuropatkin              //

/* 
This header file contains global macros used in other code files.
These macros are primarily defined to simplify the code.
For example, using words like CLIENT = 1 or SERVER = 0 improves the 
readability and understanding of the code. If 1 and 0 were used instead, 
the code could be harder to understand.
Read the comments next to each macro for an explanation of its purpose.
*/
#ifndef MACROS_H
#define MACROS_H

/*
Two next Macros are used for easier understanding of code,
some functions have different function order or print different
strings depending on which side called it(Server or Client)
These Macros are in use in:
server.c: calling kdf function
crypto.c: kdf function
client.c: calling kdf function
addtion.c: help_print function
Do not change this values without reading the code 
and defining what function they do!
*/
#define SERVER 0 // Macro that defines Server side
#define CLIENT 1 // Macro that defines Client side

#endif
// Client-server api              //
// Additional functions           //
// Version 0.5.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 19.11.2024                     //
// Nikita Kuropatkin              //

/* 
This header file contains macros for the operation of the client and server programs.
Read the comments next to each macro for an explanation of its purpose.
*/
#ifndef PARAMETERS_H
#define PARAMETERS_H
/*
Message size, can be changed at your preference.
This size means amount of characters that will be readed from stdin to send it
*/
#define MAX 400    

#define KEYSZ 32   //SK, PK, Hidden PK sizes
#define NONSZ 24   //Nonce size
#define MACSZ 16   //MAC size
/*
Changing this sizes can and will create security risks or program instability!
If you need different key or nonce sizes, pls read whole code before
and consider using different functions from Monocypher
*/

#define PORT 8087 // port number

#endif
// Client-server api              //
// Additional functions           //
// Version 0.6                    //
// Bachelor`s work project        //
// Technical University of Kosice //
// 28.11.2024                     //
// Nikita Kuropatkin              //

/* 
This header file contains macros for the operation 
of the client and server programs.
Read the comments next to each macro for an explanation of its purpose.
*/
#ifndef PARAMETERS_H
#define PARAMETERS_H

/*
Message size, can be changed at your preference.
This size means amount of characters that will be readed from 
stdin to send it.
*/
#define TEXT_MAX 400 

/*
Actual maximum size that can be contained in buffers for 
compressed/encrypted text, it is bigger than text size, 
cause LZRW3a can expand inputed text, but it`s highly unlikely
*/
#define MAX 500

/*
Little-bit confusing name, but actually it is size 
of uint8_t array, that will contain uint32_t number
*/
#define BYTE_ARRAY_SZ 4

#define KEYSZ 32   // SK, PK, Hidden PK sizes
#define NONSZ 24   // Nonce size
#define MACSZ 16   // MAC size
/*
Changing this sizes can create security risks or program instability!
If you need different key or nonce sizes, pls read whole code before
and consider using different functions from Monocypher
*/

/*
Default port number value, will be used if user didn`t specify another
port number while running program.
*/
#define PORT 8087 

#endif
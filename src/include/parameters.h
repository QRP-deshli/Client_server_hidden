// Client-server api              //
// Parameters                     //
// Version 0.7.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 20.12.2024                     //
// Nikita Kuropatkin              //

/* 
This header file contains macros for the operation 
of the client and server programs.
Read the comments next to each macro for an explanation of its purpose.
*/
#ifndef PARAMETERS_H
#define PARAMETERS_H

/////////////////////
/* Can be modified */
/////////////////////

/*
Macros that are in this block can be modified by the user. 
In some cases, conditions need to be met, while other macros 
have no specific rules for modification.
*/

/*
In use: client.c, server.c.
Defines the message size. You can modify this value as desired.
This size represents the maximum number of characters read from 
stdin to send. Ideally, it should be 10% smaller than the macro MAX 
(minimal difference should be 5% for sizes greater than 100 characters).
*/
#define TEXT_MAX 400 

/*
In use: client.c, server.c, compress_decompress.c.
Defines the maximum size for buffers containing compressed/encrypted text. 
It is larger than TEXT_MAX since LZRW3-A can expand the input text, though 
this is highly unlikely. You can modify this value, but it should ideally 
be 10% larger than TEXT_MAX (minimal difference should be 5% for sizes 
greater than 100 characters).
*/
#define MAX 500

/*
In use: client.c, server.c.
Defines the size of a uint8_t array containing a uint32_t number. 
You can modify this, but it is unnecessary since the message size 
is unlikely to exceed the 2^33-1 boundary.
*/
#define BYTE_ARRAY_SZ 4

/*
In use: client.c, server.c; affects pin.c, crypto.c, pin_changer.c.
These values define sizes for keys, nonces, and MACs. 
Modifying them could create security risks or instability.
If changes are necessary, review the code and consider using 
different Monocypher functions.
*/
#define KEYSZ 32   // SK, PK, Hidden PK sizes
#define NONSZ 24   // Nonce size
#define MACSZ 16   // MAC size

/*
In use: client.c.
Defines the default port number. If the user does not specify a 
port while running the program, this value is used. Can be modified, 
but it should remain within the range of 0-65535. Staying in the 
classic range (1024-65535) is recommended since ports below 1024 are 
used by well-known services like DHCP, DNS, or SSH.
*/
#define PORT 8087 

/*
In use: client.c.
Defines the range of ports that the user can choose from. You can 
modify these values, but they must meet the following conditions:
# PORT_START <= PORT_END
# 0 <= PORT_START <= 65535
# 0 <= PORT_END <= 65535
*/
#define PORT_START 1024 // First available port
#define PORT_END 65535  // Last available port

/*
In use: client.c.
Specifies the default server IP address (loopback). If the user does 
not provide an address while running the program, this address is used. 
Can be modified, but it must be a valid IP in dotted decimal format.
*/
#define IP "127.0.0.1"

/*
In use: server.c.
Defines the backlog for the `listen()` function, specifying the maximum 
number of pending connections the system queues for a socket. Can be 
modified but must meet the conditions:
# BACKLOG > 0
# Recommended to keep the value below 10.
*/
#define BACKLOG 3

/*
In use: server.c.
Specifies the path to the server's long-term shared key file. 
The path is relative to the location of the program's .exe file. 
You should modify this if you change the key file's location.
*/
#define KEY_PATH_SERVER "src/server/server_key.txt"

/*
In use: addition.c.
Defines a stop-word that terminates a conversation. You can modify 
this value. If the stop-word differs on one side, the other side will 
interpret it as an error.
*/
#define EXIT "exit"

/*
In use: compress_decompress.c, pin.c.
Defines the memory allocation type for PIN hashing and text compression. 
Options: STATIC_BSS, STATIC_STACK, DYNAMIC. Adjusting the BLOCK_AMOUNT 
macro is necessary when using STATIC_STACK due to stack size limits. 
*/
#define ALLOCATION DYNAMIC  

/*
In use: config.c.
Defines where error messages are printed (stdout or stderr). 
Set to OUTPUT for stdout or ERR for stderr.
*/
#define PRINT OUTPUT 

/*
In use: config.c.
Enables or disables error printing. Set to NO to disable error 
printing, or YES to enable it.
*/
#define DEBUG NO 

/*
In use: pin.c.
Specifies the path to the client's long-term shared key file secured by a PIN. 
Modify this if the key file location changes.
*/
#define KEY_PATH_CLIENT "src/client/client_key.txt"

/*
In use: pin.c.
Specifies the path to the salt file for PIN hashing. Modify this 
if the salt file location changes.
*/
#define SALT_PATH "src/client/salt.txt"

/*
In use: pin.c.
Defines the number of memory blocks for Argon2i. The default value 
is 100 MB with 3 iterations. Adjusting this value requires 
consideration of system RAM and Argon2 input parameters.
*/
#define BLOCK_AMOUNT 100000

/*
In use: pin.c.
Defines the number of iterations for Argon2i. The default value 
is 100 MB with 3 iterations. Adjusting this requires considering system RAM 
and Argon2 input parameters.
*/
#define ITERATIONS 3

/*
In use: pin.c, pin_changer.c.
Defines the PIN size. Changing this requires reapplying a new PIN to 
the long-term shared key. Update the PIN in the corresponding .txt file.
*/
#define PINSZ 6 

/*
In use: pin.c.
Defines the salt size for PIN hashing. Changing this requires reapplying 
the PIN to the long-term shared key and updating the salt in the .txt file.
*/
#define SALTSZ 16 // Size of the salt

/////////////////////
/////////////////////

/////////////////// 
/* Do not modify */ 
///////////////////

/*
Macros that are in this block either cannot be modified by the user, 
or there is no need to modify them, like macros that define 
options for other macros.
*/

/*
In use: server.c, client.c
Structure for TCP/IP functions contains IP and port, etc., used by the client
and server to establish a connection.
Never change this value!
*/
#define SA struct sockaddr 

/*
In use: client.c
Size of IP address.
Never change the value of this macro!
*/
#define IPSZ 16  

/*
In use: addition.c
The next two macros define the allowed range 
of values for octets in IPv4 (0.0.0.0 - 255.255.255.255).
Never change the values of these macros!
*/ 
#define IPSTART 0
#define IPEND 255

/*
In use: here (upper segment -> macro ALLOCATION), also compress_decompress.c,
pin.c
Macros define types of memory allocation the user can use 
for PIN hashing and compressing and decompressing text.
You can change the values of the options, but I don't think you'll need to,
which is why they are here. (I do not recommend changing them.)
*/
#define STATIC_BSS 0    // Memory will be allocated statically in the BSS 
#define STATIC_STACK 1  // Memory will be allocated statically on the stack
#define DYNAMIC 2       // Memory will be allocated dynamically on the heap

/*
In use: here (upper segment -> macro DEBUG), config.c
Two macros define DEBUG macro options. Again, you don’t need to change
these, but you can. (I do not recommend changing them.)
*/
#define YES 1
#define NO 0

/*
In use: here (upper segment -> macro PRINT), config.c
Two macros define PRINT macro options. Again, you don’t need to change
these, but you can. (I do not recommend changing them.)
*/
#define OUTPUT 1
#define ERR 0

/*
In use: pin.c, compress_decompress.c
Macro function to dynamically allocate memory for Argon2i and LZRW3-A.
Never change this value!
*/
#define ALLOCATE_WORK_AREA(size) malloc(size) 

/*
In use: pin.c, compress_decompress.c
Macro function to de-allocate work_area memory.
If the ALLOCATION macro is set to STATIC_BSS or STATIC_STACK, it does nothing.
If the ALLOCATION macro is set to DYNAMIC, it frees the allocated memory.
Never change the values of these macros!
*/
#if ALLOCATION == DYNAMIC
  #define FREE_WORK_AREA(ptr) free(ptr)
#else
  #define FREE_WORK_AREA(ptr)  // No action for STATIC allocation
#endif

/*
The next two macros are used for easier understanding of the code.
Some functions have different function orders or print different
strings depending on which side called them (Server or Client).
These macros are in use in:
server.c: calling the KDF function
crypto.c: KDF function
client.c: calling the KDF function
addition.c: help_print function
Do not change these values without reading the code 
and determining their function!
*/
#define SERVER 0 // Macro that defines the Server side
#define CLIENT 1 // Macro that defines the Client side

/*
In use: pin.c, pin_changer.c
The produced value of the hash of a PIN should be equal to the key size, 
which is why the macro below is mapped to KEYSZ.
Do not change this value!
*/
#define HASHSZ KEYSZ 

/*
In use: pin.c.
Defines the number of threads (parallelism) for Argon2i.  
Since Monocypher uses a single-threaded approach, 
this value does not need to be changed.
*/
#define LANSES 1

/*
In use: client.c, server.c
Defining macro for address length
(it is needed cause it has different types on WIN and LINUX).
Do not change this value!
*/
#ifdef _WIN32 
    #define LEN int 
#else
    #define LEN socklen_t
#endif 

//////////////////////// 
////////////////////////

#endif

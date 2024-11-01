#ifndef SHARED_H
#define SHARED_H
#include<math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "monocypher.h"
#ifdef _WIN32
    #define _CRT_RAND_S //rand_S
    #include <winsock2.h>   //socket
    #include <ws2tcpip.h>   //socket
    #include <stdlib.h>
    #include <stdint.h>
    #include <io.h>
//Linux Libs
#else
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <netinet/in.h> // socket
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <bsd/stdlib.h>
#endif


#include "error.h" //all errors defined + function proto
//////////////////////////////////////////
/// Clearing input   ///
//////////////////////////////////////////
void clear ();
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

//////////////////////////////////////////
/// Data sender ///
//////////////////////////////////////////
/*
This function purpose is to send data over open sockets for WIN and LIN OS
*/
void read_win_lin(int sockfd, uint8_t *msg, unsigned int size);

//////////////////////////////////////////
/// Data receiver ///
//////////////////////////////////////////
/*
This function purpose is to receive data over open sockets for WIN and LIN OS
*/
void write_win_lin(int sockfd, uint8_t *msg, unsigned int size);
//////////////////////////////////////////
//////////////////////////////////////////


/////////////////
/// PADME ///
/////////////////
int padme_size(int L);

//Copy the OG PK to a Padded array, or turn back OG size to a padded PK
void pad_array(uint8_t* array, uint8_t* pad_array, int og_size, int new_size);

void unpad_array(uint8_t* array, uint8_t* pad_array, int og_size);
/////////////////
/////////////////


//////////////////////////////////////////
/// Socket closer///
//////////////////////////////////////////
/*
This function purpose is to close existing sockets for WIN and LIN OS
*/
void sockct_cls(int sockfd);
///////////////////////////////////////////////////
///////////////////////////////////////////////////

///////////////////////////////
/// Socket initializer///
//////////////////////////////
/*
This function purpose is to initialize socket for windows
*/
void init_sock();
//////////////////////////////
//////////////////////////////

///////////////////////////////
/// Socket creation checker///
//////////////////////////////
/*
This function purpose is to initialize socket for windows
*/
void sock_check(int sockfd);
//////////////////////////////
//////////////////////////////


///////////////////////////////
/// KDF with Blake2b///
//////////////////////////////
/*
This function purpose is derive shared key from 
raw shared key and PKs of both sides
*/
void kdf(uint8_t *shared_key, uint8_t *your_sk, uint8_t *their_pk, int keysz, int side);
///////////////////////////////
///////////////////////////////


/////////////////////////////////////////
/// Function for generating hidden PKs///
/////////////////////////////////////////
/*This function takes two empty arrays for keys
generates tweak for Elligator, next in infinite cycle
it generates SK and derives PK from it. If that PK
can be mapped to a random string with Elligator 2 cycle ends,
 if not it continues and new SKs and PKs are generated
 */
void key_hidden(uint8_t *your_sk, uint8_t *your_pk, int keysz);
/////////////////////////////////////////
/////////////////////////////////////////

#endif
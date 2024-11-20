// Client-server api              //
// Cryptographic functions        //
// Version 0.5.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 19.11.2024                     //
// Nikita Kuropatkin              //

/* 
This header file declares cryptographic functions
for a client-server application. Function definitions
are in crypto.c. 
*/
#ifndef CRYPTO_H
#define CRYPTO_H
#include<math.h>
#include <stdint.h>
#include <string.h>
#include "monocypher.h"
#include "random.h"	//PRNG

/////////////////
/// PADME ///
/////////////////
/*
This function purpose is to derive size of padded array from original
message size. This functions implements PADME algorithm without rounding
with bitmask(PADME: https://lbarman.ch/blog/padme/)
*/
int padme_size(int L);

//Padding of array(copying to an array of bigger size 
//and additional space is filled with random data)
void pad_array(uint8_t* array, uint8_t* pad_array, int og_size, int new_size);

//Reverse of pad_array
void unpad_array(uint8_t* array, uint8_t* pad_array, int og_size);
/////////////////
/////////////////

///////////////////////////////
/// KDF with Blake2b///
//////////////////////////////
/*
This function purpose is to derive shared key from 
raw shared key and PKs of both sides.
The last parameter(int side) is a switch to use specific 
order for a specific side(server and client),basically it 
defines what side called this function
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
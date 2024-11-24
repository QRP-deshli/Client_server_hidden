// Client-server api              //
// Cryptographic functions        //
// Version 0.5.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 19.11.2024                     //
// Nikita Kuropatkin              //

#include"crypto.h" //Crypto primitievs
#include "monocypher.h"
#include "random.h" //CSPRNG
/*Two next Macros are used for easier understanding of code,
some functions have different function order or print different
strings depending on which side called it(Server or Client)*/
#define SERVER 0 // Macro that defines Server side
#define CLIENT 1 // Macro that defines Client side

/////////////////
/// PADME ///
/////////////////
/*
This function purpose is to derive size of padded array from original
message size. This functions implements PADME algorithm without rounding
with bitmask(PADME: https://lbarman.ch/blog/padme/)
*/
int padme_size(int L) {
    //modified PADME for padding of the key + nonce
    int E = (int)log2(L);
    int S = (int)log2(E) + 1;
    int lastBits = E - S;
    int bitMask = (1 << lastBits) - 1;
    return (L + bitMask);
}

/*
Padding of array(copying to an array of bigger size 
and additional space is filled with random data)
*/
void pad_array(uint8_t* array, uint8_t* pad_array, int og_size, int new_size) {

  memcpy(pad_array, array, og_size);
  if (new_size > og_size)random_num(&pad_array[og_size], new_size-og_size);

}

//Reverse of pad_array
void unpad_array(uint8_t* array, uint8_t* pad_array, int og_size) {
    memcpy(array, pad_array,og_size);
}
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
void kdf(uint8_t *shared_key, uint8_t *your_sk, uint8_t *their_pk, int keysz, int side){
    uint8_t shared_secret[keysz]; //raw shared key
    uint8_t your_pk[keysz]; //your PK
    // Compute PK(again) but without inverse mapping
    crypto_x25519_dirty_small(your_pk, your_sk);
        
    // Compute shared secret
    crypto_x25519(shared_secret, your_sk, their_pk);

    // KDF with blake2
    crypto_blake2b_ctx ctx;
    crypto_blake2b_init(&ctx, keysz);
    crypto_blake2b_update(&ctx, shared_secret, keysz);
    if(side == SERVER){
    crypto_blake2b_update(&ctx, your_pk, keysz);
    crypto_blake2b_update(&ctx, their_pk, keysz);
    }
    else if(side == CLIENT){
        crypto_blake2b_update(&ctx, their_pk, keysz);
        crypto_blake2b_update(&ctx, your_pk, keysz);
    }
    crypto_blake2b_final(&ctx, shared_key);    // Shared key for encryption
        
    //cleaning
    crypto_wipe(shared_secret, keysz);
}
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
void key_hidden(uint8_t *your_sk, uint8_t *your_pk, int keysz){
    uint8_t tweak; // Tweak for elligator`s inverse map
    random_num(&tweak, 1);// Tweak generating

    //Cycle for creating SK and computing PK, then inverse mapping it to a scalar
    while (1) {
    random_num(your_sk, keysz);
    crypto_x25519_dirty_small(your_pk, your_sk);
    if (crypto_elligator_rev(your_pk, your_pk, tweak) == 0)
        break;
    }
}
/////////////////////////////////////////
/////////////////////////////////////////
// Client-server api              //
// PIN functions                  //
// Version 0.6.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 05.12.2024                     //
// Nikita Kuropatkin              //

/* 
This header file declares functions for securing authentication 
key by PIN for a client-server application. Function bodies
are in pin.c. Also this header is in use by pin_changer.c program, 
that helps to generate keys secured by PIN 
*/
#ifndef PIN_H
#define PIN_H
#include <stdint.h>
#include "../parameters.h" //for consitent sizes of hash and key

/*
Both of these macros will be used in pin_changer.c too
*/
#define PINSZ 6 //Size of PIN
#define SALTSZ 16 // Size of salt

/*
Produced value of Hash of PIN should be equal to key size, 
thats why macro below is mapped to KEYSZ
(defined in parameters.h size of keys)
*/
/*Size of Hash(Will be used in pin.c and pin_changer.c)*/
#define HASHSZ KEYSZ 

/////////////////////////////////////////////////
/// Function that XORs hashed pin with key    ///
/////////////////////////////////////////////////
/*
This function takes inputed key and hashed_pin 
and XOR every element of them.
It provides transforming a plain key to a key secured by PIN 
or key secured by PIN to plain.
*/
void xor_with_key(uint8_t *result_key, const uint8_t *working_key, const uint8_t *hashed_pin);
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/// Function that hashes pin with Argon2i     ///
/////////////////////////////////////////////////
/*
This function purpose is to take inputed pin and hash it using Argon2i
Value of hash will be written to vatiable hashed_pin.
Parameters for Argon are defined and commented in code
(can be modified, but very carefully)
*/
void hashing_pin(uint8_t *pin, uint8_t *hashed_pin, uint8_t *salt);
/////////////////////////////////////////////////
/////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// Function that asks users PIN for long-term SK       ///
///////////////////////////////////////////////////////////
/*
This function purpose is to ask user for his PIN, entered PIN will be
XORed with key material stored it client\secret.h, if PIN is right
client side will be authenticated, wheater it is not communacation will
end
*/
void pin_cheker(uint8_t *plain_key);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

#endif
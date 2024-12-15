// Client-server api              //
// PIN functions                  //
// Version 0.6.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 05.12.2024                     //
// Nikita Kuropatkin              //

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/client/pin.h"
#include "../include/monocypher.h"
#include "../include/parameters.h"
#include "../include/error.h"
#include "../include/txt_reader.h"

/*Path to a txt file of clients key(secured by PIN)*/
#define KEY_PATH "src/client/client_key.txt"
/*Path to a txt file of salt*/
#define SALT_PATH "src/client/salt.txt"

#define BLOCK_AMOUNT 100000 // Define the number of blocks for Argon2

/*Function to allocate memory for Argon`s work_area*/
#define ALLOCATE_WORK_AREA(size) malloc(size) 
/*Function to free memory of Argon`s work_area*/
#define FREE_WORK_AREA(ptr)      free(ptr) 

/////////////////////////////////////////////////
/// Function that XORs hashed pin with key    ///
/////////////////////////////////////////////////
/*
This function takes inputed key and hashed_pin 
and XOR every element of them.
It provides transforming a plain key to a key secured by PIN 
or key secured by PIN to plain.
*/
void xor_with_key(uint8_t *result_key, const uint8_t *working_key, const uint8_t *hashed_pin)
{
 for (size_t i = 0; i < HASHSZ; i++) {
    result_key[i] = working_key[i] ^ hashed_pin[i];
 }
}
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
void hashing_pin(uint8_t *pin, uint8_t *hashed_pin, uint8_t *salt) {
 /*Configuring Argon2 for PIN hashing*/
 crypto_argon2_config config = {
    .algorithm = CRYPTO_ARGON2_I, /* Variant of Argon*/
    .nb_blocks = BLOCK_AMOUNT,    /* The number of blocks for work area*/
    .nb_passes = 3,               /* 3 iterations*/
    .nb_lanes  = 1                /* Single-threaded*/
 };
 crypto_argon2_inputs inputs = {
    .pass      = pin,                   /* User PIN*/
    .salt      = salt,                  /* Salt for the PIN*/
    .pass_size = PINSZ,                 /* PIN length*/
    .salt_size = SALTSZ                 /* salt length*/
 };       
 crypto_argon2_extras extras = {0};   /* Extra parameters unused */

 void *work_area = ALLOCATE_WORK_AREA((size_t)BLOCK_AMOUNT * 1024);
 if (work_area == NULL) {
    crypto_wipe(pin, PINSZ);//wiping PIN, cause it`s not longer needed
    crypto_wipe(salt, SALTSZ); //wiping salt, cause it`s not longer needed
    exit_with_error(ALLOCATION_ERROR,"Memory allocation failed");
 }
 else {
    crypto_argon2(hashed_pin, HASHSZ, work_area,config, inputs, extras);
    crypto_wipe(pin, PINSZ); //wiping PIN, cause it`s not longer needed
    crypto_wipe(salt, SALTSZ); //wiping salt, cause it`s not longer needed
    FREE_WORK_AREA(work_area); //free memory after usage of Argon
 }
}
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
void pin_cheker(uint8_t *plain_key) {
 char pin[NONSZ]; // bigger size for checking
 uint8_t hashed_pin[HASHSZ]; //hashed value of pin
 /*Key secured by PIN that contained in src/client/key.txt*/
 uint8_t secured_key[KEYSZ];
 /*Salt for PIN hashing(contained in src/client/salt.txt)*/
 uint8_t salt[SALTSZ];

 printf("Enter PIN: ");
 fgets(pin, NONSZ, stdin); // getting pin

/*
 Checking if user entered digits for PIN(not other characters)
*/
 for(int i = 0; i < PINSZ; i++)
 {
    if (pin[i] < '0' || pin[i] > '9'){
       exit_with_error(WRONG_PIN_FORMAT,"PIN must be digits(0-9)!");
    }
 }

 int len = strlen(pin)-1; // length of PIN client entered

 // Buffer overflow or PIN longer that it suppose to be, abort
 if (pin[len] != '\n' || len != PINSZ) {
    exit_with_error(WRONG_PIN,"You entered wrong PIN");
 }

 read_from_txt(KEY_PATH, secured_key, KEYSZ); //Reading key from txt
 read_from_txt(SALT_PATH, salt, SALTSZ); //Reading salt  from txt

 hashing_pin((uint8_t*)pin, hashed_pin, salt); // hashing with ARGON2
 /*XORing resulted hash with key-material*/
 xor_with_key(plain_key, secured_key, (uint8_t*)hashed_pin); 
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


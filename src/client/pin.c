// Client-server api              //
// PIN functions                  //
// Version 0.6                    //
// Bachelor`s work project        //
// Technical University of Kosice //
// 28.11.2024                     //
// Nikita Kuropatkin              //

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pin.h"
#include "../monocypher.h"
#include "secret.h"
#include "../parameters.h"
#include "../error.h"

#define SALTSZ 16 // Size of dalt
#define BLOCK_AMOUNT 100000 // Define the number of blocks for Argon2

#define ALLOCATE_WORK_AREA(size) malloc(size) //function to allocate memory for Argon`s work_area
#define FREE_WORK_AREA(ptr)      free(ptr) //function to free memory of Argon`s work_area

/*
 Salt for hashing(can be changed, but you`ll need to update secured_key
 with pin_changer.c(MODE = 0) + secured_key in src/client/secret.h
 needs to be set as a plain key
 (same key as server uses, typically key, that wasn`t secured by PIN)).
*/
uint8_t salt[SALTSZ] = {
    0xF3,0x47,0x75,0xC8,
    0xEE,0x2B,0x9D,0x1D,
    0x3A,0xE3,0x90,0xFA,
    0xBF,0x44,0xAE,0x0D
};

/////////////////////////////////////////////////
/// Function that XORs hashed pin with key    ///
/////////////////////////////////////////////////
/*
This function takes inputed key and hashed_pin and XOR every element of them.
It provides transforming a plain key to a key secured by PIN or key secured by PIN to plain
*/
void xor_with_key(const uint8_t *hashed_pin, uint8_t *result_key, uint8_t *working_key) {
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
Parameters for Argon are defined and commented in code(can be modifies, but very carefully)
*/
void hashing_pin(uint8_t *pin, uint8_t *hashed_pin) {
    /*Configuring Argon2 for PIN hashing*/
    crypto_argon2_config config = {
        .algorithm = CRYPTO_ARGON2_I,            /* Variant of Argon we`ll use */
        .nb_blocks = BLOCK_AMOUNT,               /*The number of blocks for the work area */
        .nb_passes = 3,                          /* 3 iterations */
        .nb_lanes  = 1                           /* Single-threaded */
    };
    crypto_argon2_inputs inputs = {
        .pass      = pin,                   /* User PIN */
        .salt      = salt,                  /* Salt for the PIN */
        .pass_size = PINSZ,                 /* PIN length */
        .salt_size = SALTSZ                 /* salt length */
    };       
    crypto_argon2_extras extras = {0};   /* Extra parameters unused */

    void *work_area = ALLOCATE_WORK_AREA((size_t)BLOCK_AMOUNT * 1024);
    if (work_area == NULL) {
        exit_with_error(ALLOCATION_ERROR,"Memory allocation failed");
        crypto_wipe(pin, PINSZ);//wiping PIN, cause it`s not longer needed
    }
    else {
        crypto_argon2(hashed_pin, HASHSZ, work_area,
                      config, inputs, extras);
        crypto_wipe(pin, PINSZ); //wiping PIN, cause it`s not longer needed
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

    printf("Enter PIN: ");
    fgets(pin,NONSZ,stdin); // getting pin
    int len = strlen(pin)-1; // length of PIN client entered

    // Buffer overflow or PIN longer that it suppose to be, abort
    if (pin[len] != '\n' || len != PINSZ) {
        exit_with_error(WRONG_PIN,"You entered wrong PIN");
    }

    hashing_pin((uint8_t*)pin,hashed_pin); // hashing with ARGON2
    xor_with_key((uint8_t*)hashed_pin, plain_key, secured_key); //xoring resulted hash with key-material

}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


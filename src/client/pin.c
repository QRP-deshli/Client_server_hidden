// Client-server api              //
// PIN functions                  //
// Version 0.7.0                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 16.12.2024                     //
// Nikita Kuropatkin              //

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/client/pin.h"
#include "../include/monocypher.h"
#include "../include/parameters.h"
#include "../include/error.h"
#include "../include/txt_reader.h"

/*!DEMO!*/
/* Macros for deciding the type of allocation */
#define STATIC 0
#define DYNAMIC 1
#define ALLOCATION STATIC  // Set ALLOCATION to STATIC or DYNAMIC

/*Path to a txt file of clients key(secured by PIN)*/
#define KEY_PATH "src/client/client_key.txt"
/*Path to a txt file of salt*/
#define SALT_PATH "src/client/salt.txt"

#define BLOCK_AMOUNT 100000 // Define the number of blocks for Argon2

/*Function to allocate memory for Argon`s work_area*/
#define ALLOCATE_WORK_AREA(size) malloc(size) 

/*
 Macro function to de-allocate work_area memory.
 If ALLOCATION is set to STATIC, does nothing.
 If ALLOCATION is set to DYNAMIC, frees the allocated memory.
*/
#if ALLOCATION == DYNAMIC
  #define FREE_WORK_AREA(ptr) free(ptr)
#else
  #define FREE_WORK_AREA(ptr)  // No action for STATIC allocation
#endif

/*
This function takes an input key and a hashed PIN, and performs an 
XOR operation on each corresponding element of both arrays. 
The function can be used to:
- Transform a plain key into a key secured by a PIN.
- Convert a key secured by a PIN back to its original plain form.
Parameters:
- `result_key`: A pointer to the buffer where the result of the 
   XOR operation will be stored.
- `working_key`: A pointer to the working key that will be XORed with 
  the hashed PIN.
- `hashed_pin`: A pointer to the hashed PIN that will be used for the 
  XOR operation.
Each byte of the `working_key` is XORed with the corresponding byte of the 
`hashed_pin` to generate the `result_key`.
*/
void xor_with_key(uint8_t *result_key, const uint8_t *working_key, const uint8_t *hashed_pin)

{
 for (size_t i = 0; i < HASHSZ; i++) {
    result_key[i] = working_key[i] ^ hashed_pin[i];
 }
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/*
This function takes an input PIN and hashes it using the Argon2i algorithm. 
The Argon2i parameters used in this function are predefined in the code and 
can be modified, but changes should be made with caution to ensure security 
and proper functioning.
Parameters:
- `pin`: A pointer to the input PIN that will be hashed.
- `hashed_pin`: A pointer to the buffer where the resulting hashed PIN will 
  be stored.
- `salt`: A pointer to the salt that will be used in the hashing process.
The function uses Argon2i to hash the `pin` with the provided `salt` and 
stores the result in the `hashed_pin`.
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

 /*
  Working memory for LZRW3a compression.
  The allocation type is determined based on the ALLOCATION flag.
  In future versions, this should be refactored to allow decisions at 
  preprocessing time.
 */
 #if ALLOCATION == DYNAMIC
   void *work_area = ALLOCATE_WORK_AREA((size_t)BLOCK_AMOUNT * 1024);
   if (work_area == NULL) {
    crypto_wipe(pin, PINSZ);//wiping PIN, cause it`s not longer needed
    crypto_wipe(salt, SALTSZ); //wiping salt, cause it`s not longer needed
    exit_with_error(ALLOCATION_ERROR,"Memory allocation failed");
   }
 #else
   static uint8_t work_area[BLOCK_AMOUNT * 1024] __attribute__((aligned(8)));
 #endif
 crypto_argon2(hashed_pin, HASHSZ, work_area,config, inputs, extras);
 crypto_wipe(pin, PINSZ); //wiping PIN, cause it`s not longer needed
 crypto_wipe(salt, SALTSZ); //wiping salt, cause it`s not longer needed
 FREE_WORK_AREA(work_area); //free memory after usage of Argon
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/*
This function prompts the user to enter their PIN. 
The entered PIN is then XORed with key material stored in `client/secret.h`.
If the PIN is correct, the client side will be authenticated. 
If the PIN is incorrect, the communication will end.
Parameters:
- `plain_key`: A pointer to the key material that will be XORed with 
the entered PIN. This key is used for authentication.
The function ensures that only users with the correct PIN can authenticate 
successfully, allowing further communication to proceed.
*/
void pin_checker(uint8_t *plain_key) {
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
 for (int i = 0; i < PINSZ; i++) {
    if (pin[i] < '0' || pin[i] > '9') {
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


// Client-server api              //
// PIN functions                  //
// Version 0.7.0                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 16.12.2024                     //
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
void xor_with_key(uint8_t *result_key, const uint8_t *working_key, const uint8_t *hashed_pin);
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
void hashing_pin(uint8_t *pin, uint8_t *hashed_pin, uint8_t *salt);
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
void pin_checker(uint8_t *plain_key);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

#endif
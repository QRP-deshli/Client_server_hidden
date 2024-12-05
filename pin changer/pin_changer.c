// Add-on for API                 //
// PIN changer                    //
// Version 0.2                    //
// Bachelor`s work project        //
// Technical University of Kosice //
// 05.12.2024                     //
// Nikita Kuropatkin              //

/*
This program helps client to secure key by PIN
There are two modes for this program:

SECURE_KEY mode - secures key in src/client/key.txt by PIN, 
so if you want to secure freshly generated key, 
add it to servers side(src/server/secret.h) and also add it to 
src/client/key.txt, after that run this program with MODE set to 
SECURE_KEY, it will print key secured by PIN to stdout, 
copy it and refresh variable in src/client/key.txt with it.

CHANGE_PIN mode - in this mode program takes key already secured by 
old_pin from src/client/key.txt and secures it by new PIN;
it will print key secured by new PIN to stdout, 
copy it and refresh variable in src/client/key.txt with it.

In both modes do not forget to change value of new PIN in new_pin.txt 
and if you changing PIN for key update value in old_pin.txt also. 
*/

//////////Version history//////////
/*
Version 0.2 :
# Added reading from txt files, without need of recompiling program after 
  changing PIN for a key
Version 0.1 - basic functionality
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "monocypher.h"
#include "client/pin.h"
#include "txt_reader.h"

/*
Macros below defines mode of this program

Set MODE to SECURE_KEY if you want to just to secure plain key
(key that wasnt secured by pin before)

Set MODE to CHANGE_PIN if you want to change pin for key
(delete old pin and apply new)
*/
#define SECURE_KEY 0
#define CHANGE_PIN 1
#define MODE CHANGE_PIN

/*Path to a txt file of clients key*/
#define KEY_PATH "../src/client/key.txt"
/*Path to a txt file of salt*/
#define SALT_PATH "../src/client/salt.txt"
/*Path to a txt file of new PIN*/
#define NEW_PIN_PATH "new_pin.txt"
/*Path to a txt file of old PIN*/
#define OLD_PIN_PATH "old_pin.txt"

//////////////////////////////
/// Secured key printer    ///
//////////////////////////////
/*
This function takes input a variable that points to key and prints it 
to stdout in text form. You need to copy(manually replace) new key to 
src/client/key.txt
*/
static void print_key(const uint8_t *finish_key){
 printf("!New key-material!\n");
 printf("(Actual authentication key can be accessed by xoring PIN to this value)\n");
 printf("Manually replace value in src/client/key.txt by this value:\n");

 /*
  Loop for printing secured key in beautified form,
  whole loop can be changed to standart printing every element of 
  finish_key: 
  	for(int i = 0; i< HASHSZ;i++)printf("%02X",finish_key[i])
 */	
 printf("\n");
 for (int i = 0; i < HASHSZ/8; i++) { 
	printf(" ");
	for(int j = 0; j < 8; j++) {
		if(i!=(HASHSZ/8)-1 || j!=7)printf(" %02X",finish_key[i*8+j]);
		else printf(" %02X",finish_key[i*8+j]);
	}
	printf("\n");
	}
}
//////////////////////////////
//////////////////////////////

////////////////////////////////////////////
/// Function that secures key with PIN   ///
////////////////////////////////////////////
/*
This function uses value of new_pin to secure key,
that is contained in src/client/secret.h.
Remember to change value contained in new_pin.txt to your new PIN, 
also rember to copy servers key_original value to value stored
in src/client/key.txt.
This way you will be securing plain key(right way).
Simply: it applies PIN for your key.
*/
static void pin_to_plain(const uint8_t *shared_key, uint8_t *salt, uint8_t *new_pin)
{
 uint8_t hashed_new_pin[HASHSZ]; //hash of new pin
 uint8_t new_pinned_key[HASHSZ]; //new key secured by NEW pin

 hashing_pin(new_pin, hashed_new_pin, salt);//hashing NEW pin
 /*Getting pinned key from plain key*/
 xor_with_key(new_pinned_key, shared_key, hashed_new_pin); 

 print_key(new_pinned_key); //printing secured key
}
////////////////////////////////////////////
////////////////////////////////////////////

//////////////////////////////////////////////
/// Function that changes PIN for the key  ///
//////////////////////////////////////////////
/*
This function uses value of old_pin to un-secure key,
that is contained in src/client/key.txt, 
than secures that key with new PIN(new_pin.txt)
Remember to change value contained in new_pin.txt to your new PIN, 
and old_pin.txt to your old PIN.
Simply: it changes PIN for your key.
*/
static void apply_new_pin(const uint8_t *shared_key, uint8_t *salt, uint8_t *new_pin, uint8_t *old_pin){
 uint8_t plain_key[HASHSZ]; //Plain key, key that is not secured by pin
 uint8_t new_pinned_key[HASHSZ]; //New key secured by NEW pin
 uint8_t hashed_old_pin[HASHSZ]; //Hash of old pin
 uint8_t hashed_new_pin[HASHSZ]; //Hash of new pin

 hashing_pin(old_pin, hashed_old_pin, salt); //hashing OLD pin
 /*Getting plain key*/
 xor_with_key(plain_key, shared_key, hashed_old_pin);
 /*Getting pinned key from plain key*/
 hashing_pin(new_pin, hashed_new_pin, salt); //hashing NEW pin

 xor_with_key(new_pinned_key, plain_key, hashed_new_pin); 

 print_key(new_pinned_key); //printing secured key
}
//////////////////////////////////////////////
//////////////////////////////////////////////

///////////////////////////////////
/// Caster from uint8_t to char ///
///////////////////////////////////
/*
This function casts uint8_t array values 
to their representation in char(ASCII)
*/
void cast(uint8_t *pin){
 for(int i = 0; i < PINSZ;i++) pin[i]+='0';
}
///////////////////////////////////
///////////////////////////////////

/*main calling functions*/
int main()
{
 /*
 Key that contained in src/client/key.txt
 (can be plain or secured by PIN)
 */
 uint8_t shared_key[HASHSZ]; 
 /*Salt for PIN hashing(contained in src/client/salt.txt)*/
 uint8_t salt[SALTSZ]; 
 /*Your new PIN, that is contained in new_pin.txt(current folder)*/
 uint8_t new_pin[PINSZ]; 

 read_from_txt(KEY_PATH, shared_key, HASHSZ); //Reading key from txt
 read_from_txt(SALT_PATH, salt, SALTSZ); //Reading salt from txt
 read_from_txt(NEW_PIN_PATH, new_pin, PINSZ); //Reading new PIN from txt
 cast(new_pin); //Casting PIN to char values(from uint8_t value)

 /*Calling functions by MODE value*/
 if(MODE == SECURE_KEY) {
 	pin_to_plain(shared_key, salt, new_pin);
 }
 else if (MODE == CHANGE_PIN){
 	/*Your old PIN, that is contained in old_pin.txt(current folder)*/
 	uint8_t old_pin[PINSZ]; 
 	read_from_txt(OLD_PIN_PATH, old_pin, PINSZ); //Reading old PIN
 	cast(old_pin); //Casting PIN to char values(from uint8_t value)
 	apply_new_pin(shared_key, salt, new_pin, old_pin);
 } 

 getchar();
 return 0;
}
// Add-on for API                 //
// PIN changer                    //
// Version 0.1                    //
// Bachelor`s work project        //
// Technical University of Kosice //
// 26.11.2024                     //
// Nikita Kuropatkin              //

/*
This program helps client to secure key by PIN
There are two modes for this program:

SECURE_KEY mode - secures key in src/client/secret.h by PIN, 
so if you want to secure freshly generated key, add it to servers side(src/server/secret.h)
and also add it to src/client/secret.h, after that run this program with MODE set to SECURE_KEY, 
it will print key secured by PIN to stdout, copy it and refresh variable in src/client/secret.h with it.

CHANGE_PIN mode - in this mode program takes key already secured by old_pin from src/client/secret.h and secures it by new PIN
it will print key secured by new PIN to stdout, copy it and refresh variable in src/client/secret.h with it.

In both modes do not forget to change variables of old_pin[PINSZ] and new_pin[PINSZ] to your PIN values 
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "monocypher.h"
#include "client/secret.h"
#include "client/pin.h"

/*Macos below defines mode of this program
set MODE to SECURE_KEY if you want to just to secure plain key(key that wasnt secured by pin before)
set to CHANGE_PIN if you want to change pin for key(delete old pin and apply new)
*/
#define SECURE_KEY 0
#define CHANGE_PIN 1
#define MODE SECURE_KEY

uint8_t new_pin[] = {'7','7','7','7','7','7'}; // old pin for client
uint8_t old_pin[] = {'7','7','7','7','7','7'}; // new pin for client

//////////////////////////////
/// Secured key printer    ///
//////////////////////////////
/*
This function takes input a variable that points to key and prints it to stdout in form of variable,
 that you need to change in secret.h
*/
void print_key(uint8_t *finish_key){
	printf("!New key-material!\n");
	printf("(Actual authentication key can be accessed by xoring the pin to this value)\n");
	printf("Replace variable in src/client/secret.h by this value:\n");

	printf("static uint8_t secured_key[32] = {\n");
	/*
	Loop for printing secured key in beautified form,
	whole loop can be changed to standart printing every element of finish_key:
	for(int i = 0; i< HASHSZ;i++)printf("%02X",finish_key[i])
	*/	
	for (int i = 0; i < HASHSZ/4; i++) { 
		printf("\t");
		for(int j = 0; j < 4; j++) {
			if(i!=(HASHSZ/4)-1 || j!=3)printf("0x%02X,",finish_key[i*4+j]);
			else printf("0x%02X",finish_key[i*4+j]);
		}
		printf("\n");
	}
	printf("};\n");
}
//////////////////////////////
//////////////////////////////

////////////////////////////////////////////
/// Function that secures key with PIN   ///
////////////////////////////////////////////
/*
This function uses value of new_pin to secure key,
that is contained in src/client/secret.h.
Remember to change variable of new_pin to your new PIN, 
also rember to copy servers key_original value to secure_key value in src/client/secret.h.
This way you will be securing plain key(right way).
Simply: it applies PIN for your key.
*/
void pin_to_plain(){
	uint8_t hashed_new_pin[HASHSZ]; //hash of new pin
	uint8_t new_pinned_key[HASHSZ]; //new key secured by NEW pin

	hashing_pin(new_pin,hashed_new_pin);//hashing NEW pin
	xor_with_key(hashed_new_pin, new_pinned_key, secured_key); //getting pinned key from plain key	

	print_key(new_pinned_key); //printing secured key
}
////////////////////////////////////////////
////////////////////////////////////////////

////////////////////////////////////////////
/// Function that secures key with PIN   ///
////////////////////////////////////////////
/*
This function uses value of old_pin to un-secure key,
that is contained in src/client/secret.h, than secures that key with new PIN(new_pin)
Remember to change variable of new_pin to your new PIN and old_pin to your old PIN.
Simply: it changes PIN for your key.
*/
void apply_new_pin(){
	uint8_t plain_key[HASHSZ]; //plain key, key that is not secured by pin
	uint8_t new_pinned_key[HASHSZ]; //new key secured by NEW pin
	uint8_t hashed_old_pin[HASHSZ]; //hash of old pin
	uint8_t hashed_new_pin[HASHSZ]; //hash of new pin

	hashing_pin(old_pin,hashed_old_pin); //hashing OLD pin
	xor_with_key(hashed_old_pin, plain_key, secured_key); //getting plain key

	hashing_pin(new_pin,hashed_new_pin); //hashing NEW pin
	xor_with_key(hashed_new_pin, new_pinned_key, plain_key); //getting key secured by NEW pin

	print_key(new_pinned_key); //printing secured key
}
////////////////////////////////////////////
////////////////////////////////////////////

/*main calling functions*/
int main()
{
	/*Calling functions by MODE value*/
	if(MODE == SECURE_KEY) pin_to_plain();
	else if (MODE == CHANGE_PIN) apply_new_pin();

	getchar();
	return 0;
}
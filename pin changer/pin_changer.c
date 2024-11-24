#include <stdio.h>
#include <stdint.h>
#include "monocypher.h"
#include "client/secret.h"

#define PINSZ 6 // size of pin
#define KEYSZ 32 // size of key
/*This macro defines mode of this program
set to 0 if you want to just to secure plain key(key that wasnt secured by pin before)
set to 1 if you want to change pin for key(delete old pin and apply new)
*/
#define MODE 0

uint8_t old_pin[PINSZ] = {'1','2','3','4','5','6'}; // old pin for client
uint8_t new_pin[PINSZ] = {'1','2','3','4','5','6'}; // new pin for client

void xor_with_key(const uint8_t *hashed_pin, uint8_t *result_key, uint8_t *working_key) {
	for (size_t i = 0; i < KEYSZ; i++) {
		result_key[i] = working_key[i] ^ hashed_pin[i];
	}
}

void print_key(uint8_t *finish_key){
	printf("!New key-material!\n");
	printf("(Actual authentication key can be accessed by xoring the pin to this value)\n");
	printf("Replace variable in src/client/secret.h by this value\n");
	printf("uint8_t key_original[32] = {\n");
	/*for (int i = 0; i < KEYSZ/4; i++) {
		printf("\t");
		for(int j = 0; j < 4; j++) {
			if(i!=(KEYSZ/4)-1 || j!=3)printf("0x%02X,",finish_key[i*4+j]);
			else printf("0x%02X",finish_key[i]);
		}
		printf("\n");
	}
	*/
	for (int i = 0; i < KEYSZ; i++) printf("0x%02X,",finish_key[i]);
	printf("};\n");
}
void pin_to_plain(){
	uint8_t hashed_new_pin[KEYSZ]; //hash of new pin
	uint8_t new_pinned_key[KEYSZ]; //new key secured by NEW pin

	crypto_blake2b(hashed_new_pin, KEYSZ, new_pin, PINSZ); //hashing NEW pin
	xor_with_key(hashed_new_pin, new_pinned_key, key_original); //getting pinned key from plain key	
	print_key(new_pinned_key); //printing secured key
}
void apply_new_pin(){
	uint8_t plain_key[KEYSZ]; //plain key, key that is not secured by pin
	uint8_t new_pinned_key[KEYSZ]; //new key secured by NEW pin
	uint8_t hashed_old_pin[KEYSZ]; //hash of old pin
	uint8_t hashed_new_pin[KEYSZ]; //hash of new pin

	crypto_blake2b(hashed_old_pin, KEYSZ, old_pin, PINSZ); //hashing OLD pin
	xor_with_key(hashed_old_pin, plain_key, key_original); //getting plain key

	crypto_blake2b(hashed_new_pin, KEYSZ, new_pin, PINSZ); //hashing NEW pin
	xor_with_key(hashed_new_pin, new_pinned_key, plain_key); //getting key secured by NEW pin
	print_key(new_pinned_key); //printing secured key
}
int main()
{
	if(MODE == 0)pin_to_plain();
	else if (MODE == 1)apply_new_pin();
	getchar();
	return 0;
}
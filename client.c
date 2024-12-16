// Client-server api              //
// Client side                    //
// Version 0.7.0                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 16.12.2024                     //
// Nikita Kuropatkin              //

/*
This code provides encrypted client-server communication in form of chat.
Also this program has steganographic support in form of usage
Elligator 2 and PADME.
Program uses Monocypher`s library provided cryptographic primitives:
Crypto:
Incremental AEAD with this two algorithms:
    xChaCha20 - stream cypher for text encryption;
    Poly1305 - one-time MAC;
Blake2b - hash function used for key derivation;
X25519 - function for key exchange(uses Curve25519);
Argon2i - algorithm for hashing passwords or kdf based on 
hash value of key;
Crypto_wipe - function for memory wipe;
Crypto_verify16 - function for constant time comparison;
Stegano:
Elligator 2 - algorithm that provides indistinguishability of 
public keys, that will be used in key exchange;
PADME - padding algorithm.
Additional:
Program uses LZRW3-A by ROSS WILLIAMS for compressions/decompression 
of messages;
This code is client side,
!REMEMBER! SERVER MUST BE RUNNED FIRST!
*/

/*
Links:
Elligator 2: https://elligator.org/
Monocypher: https://monocypher.org/
LZRW3-A: http://www.ross.net/compression/lzrw3a.html
*/

/*
Comment explanation:
SK -secret key
PK -public key
KDF - key derivation function
MAC - message authentication code
AEAD - authenticated encryption with additional data
*/

//////////Version history//////////
/*
Version 0.7.0:
# Added DEMO version for static memory allocation in compress_decompress.c.
# The long-term shared key is now read from a `.txt` file on the server 
  side as well.
# Refactored help comments: added explanations for every input value and 
  functionality.
# Migration from using one shared encryption key on both sides to two keys 
  (`writing_key` and `reading_key`) like in a DH Ratchet structure 
  (with only 2 iterations).
# Corrected some mistakes in the ReadMe and comments.
# Added a memory allocation check in the `decompress` function in 
  `compress_decompress.c`.
# Changed the structure of `padme_size` to be more lightweight, because 
  the `log2` function is now called during preprocessing.
# Migrated header files (`.h`) to `src/include`.
Version 0.6.5 :
# Added txt file-reader for reading key and salt on clients side
# Added more macros and created new file for them macros.h
# Added checking format for inputed PIN
# Corrected mistake for metadata wiping(PIN wiping after hashing)
# Usage of key-words const and static for clarification
# Code beautified: comments and codes is less wider now 
Version 0.6 :
# Added macros for sides(CLIENT and SERVER), 
  which makes code easier to understand
# Linking mistake corrected 
  (every .h and .c file can be compiled by itself)
# Structure of project changed: new .h and .c files 
  + folders (client, server)
# Added compression of inputs by LZRW3-A on both sides
# Corrected some mistakes (late key wiping - security risk)
# Additional program (pin_changer) added to project
# Added method for securing long-term shared key on client’s side 
  by PIN (using Argon2i)
# Code beautified: more comments, more macros, irrelevant things deleted
Version 0.5.5 :
# Added a demo for long-term shared key authentication
# Corrected mistake with IP handling in memory
# Cleaned up and beautified some functions
Version 0.5 : 
# Added and moved code and macros to additional files
# Migrated from ChaCha20 encryption to AEAD stream encryption
# Added help + port/ip input checks
# Cleaned up code and added more comments
# Added .bat files for easy start
Version 0.4 : 
# Added posibility to change default port and IP of server, 
  while running program
# All functions for compatibility with different OSs are contained 
  in shared.c now
# Added comments more comments and Macros
# Added functions kdf() and key_hidden(), also additional macros for them, 
    kdf() - key derivation with blake2b; 
    key_hidden()- for masking Public keys;
# Added new logic for printing errors, check config.c and error.h for 
  further explanation
# Corrected mistake with shared key
Version 0.3 : 
# Migration of shared functions to shared.c
# Correction of macros
# Structured error returning code
# Corrected problem with memory in number generation
Version 0.2 :
# Added comments, explanations etc. 
# Corrected mistake for generating random numbers in Windows 
# Functions that needed different code on different platforms 
  created(main code cleared from ifdef) 
# Warnings cleared 
# PADME padding with random numbers now(Mistake corrected) 
# Dealing with buffer overflow added 
Version 0.1 - basic functionality
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "monocypher.h"
#include "network.h"
#include "addition.h"
#include "random.h"
#include "crypto.h"
#include "parameters.h"
#include "error.h"
#include "client/pin.h"
#include "compress_decompress.h"
#include "macros.h"

/*
Default ip address of the server(Loopback), 
if user didn`t provide other address, while running program, 
than this address will be used
*/
#define IP "127.0.0.1"

#define IPSZ 16   // Ip size

//////////////////////////////////////////
/// Socket opener ///
//////////////////////////////////////////
/*
This function purpose is to open sockets for WIN and LIN OS
Return value of this function is file descriptor of socket(ID of socket)
Also input variables are port number and server IP
*/
static int sockct_opn(int port, char *ip)
{
 int sockfd;
 struct sockaddr_in servaddr;
 init_sock();

 // Socket creation and verification
 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 sock_check(sockfd);

 // Clearing the server address structure
 memset(&servaddr, 0, sizeof(servaddr)); 

 // Assign IP, PORT
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr = inet_addr(ip);
 servaddr.sin_port = htons(port);

 // Connect the client socket to the server socket
 if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
     sockct_cls(sockfd);
     exit_with_error(ERROR_SOCKET_CREATION, "Connection with the server failed");
    } 
 else {
     printf("Connected to the server..\n");
 }
 return sockfd;
}
///////////////////////////////////////////////////
///////////////////////////////////////////////////

///////////////////////////////////////////////////
/// Client-server communication "chatting" ///
///////////////////////////////////////////////////
static void chat(uint8_t* writing_key, uint8_t* reading_key, int sockfd)
{
 // Variables for text(plain, compressed, encrypted)
 char buff[MAX]; // Buffer for encrypted text
 char plain[TEXT_MAX]; // Buffer for decrypted(plain) text
 char compr[MAX]; // Buffer for compressed text
 uint32_t compr_size = 0; // Size of compressed text
 /*Also size of compressed text in uint8_t array(needed for sending)*/
 uint8_t compr_size_bytes[BYTE_ARRAY_SZ];
    
 // Variables for nonce
 uint8_t nonce_us[NONSZ]; // Our nonce array
 uint8_t nonce_thm[NONSZ]; // Their nonce array
 int pad_size_nonce = padme_size(NONSZ); // Size of padded nonce
 /*New array that will contain padded nonce*/
 uint8_t pad_nonce[pad_size_nonce]; // Size of padded Nonce
 /*New array that will contain their padded nonce*/
 uint8_t pad_nonce_their[pad_size_nonce]; 

 // Variables for MAC
 uint8_t mac_us[MACSZ]; // MAC of our messages
 uint8_t mac_thm[MACSZ]; // MAC of their messages
 int pad_size_mac = padme_size(MACSZ); // Size of padded MAC
 uint8_t padded_mac_us[pad_size_mac]; // Our padded MAC
 uint8_t padded_mac_thm[pad_size_mac]; // Their padded MAC

 /*AEAD state variables:*/
 /*
 Our structure for aead(stores and increments 
 Shared Key, Nonce and block counter)
 */
 crypto_aead_ctx ctx_us;
 /*
 Their structure for aead(stores and increments 
 Shared Key, Nonce and block counter)
 */
 crypto_aead_ctx ctx_thm;

 // Generate nonce
 random_num(nonce_us, NONSZ);

 // Pad Nonce
 pad_array(nonce_us, pad_nonce, NONSZ, pad_size_nonce);

 // Send/recieve nonce
 write_win_lin(sockfd, pad_nonce, pad_size_nonce);
 read_win_lin(sockfd, pad_nonce_their, pad_size_nonce);

 // Un-pad Nonce
 unpad_array(nonce_thm, pad_nonce_their, NONSZ);

 // Initialization of an AEAD states:
 crypto_aead_init_x(&ctx_us, writing_key, nonce_us);
 crypto_aead_init_x(&ctx_thm, reading_key, nonce_thm);
 /*
  AEAD structure provide dynamic re-keying with memory wipe of 
  previous key, but it would not wipe original key, that was used 
  for initializing of AEAD structure, so we need to wipe it manually 
  after initialization(read Monocypher manual for further exp.)
 */
 crypto_wipe(reading_key, KEYSZ); // Wiping original reading SK
 crypto_wipe(writing_key, KEYSZ); // Wiping original writing SK

 // Chat loop:
 while (1) {
    // Clear vars 
    memset(buff, 0, MAX);
    memset(plain, 0, TEXT_MAX);
    memset(compr, 0, MAX);
    memset(compr_size_bytes, 0, BYTE_ARRAY_SZ);
    compr_size = 0;

    // Recieve message to send
    printf("To server: ");
    if (fgets(plain, TEXT_MAX, stdin) == NULL) {
        exit_with_error(ERROR_GETTING_INPUT, "Error reading input");
    }

    // Buffer overflow, clear stdin
    if (plain[strlen(plain) - 1] != '\n') {
        printf("Your message was too long, boundaries is: %d symbols,"
               "only those will be sent.\n",TEXT_MAX);
        clear();
        plain[strlen(plain) - 1] = '\n';
    }

    // Compressing inputed text
    compress_text((uint8_t*)plain, MAX, (uint8_t*)compr, &compr_size); 

    // Encrypt compressed message and generate MAC for it
    crypto_aead_write(&ctx_us, (uint8_t*)buff, mac_us, NULL, 0,(uint8_t*)compr, compr_size);

    /*Padding our MAC*/
    pad_array(mac_us, padded_mac_us, MACSZ, pad_size_mac);
    /*Send padded MAC of our message*/
    write_win_lin(sockfd, padded_mac_us, pad_size_mac);

    // Convert size to byte array
    to_byte_array(compr_size,compr_size_bytes);
    // Send size of message to other side
    write_win_lin(sockfd, compr_size_bytes, BYTE_ARRAY_SZ);
    // Send encrypted message to server
    write_win_lin(sockfd, (uint8_t*)buff, compr_size);

    if (exiting("Client", plain) == 1) break; //Checks for stop-word
		
    // Clear buffers 
    memset(buff, 0, MAX);
    memset(compr, 0, MAX);
    crypto_wipe(plain, TEXT_MAX);// clear plain
    memset(compr_size_bytes, 0, BYTE_ARRAY_SZ);
    compr_size = 0;

    // Get padded MAC of message
    read_win_lin(sockfd, padded_mac_thm, pad_size_mac);
    /*Un-pad recieved MAC*/
    unpad_array(mac_thm, padded_mac_thm, MACSZ);

    // Get size of message from other side
    read_win_lin(sockfd, compr_size_bytes, BYTE_ARRAY_SZ);
    // Convert to uint32_t
    compr_size = from_byte_array( compr_size_bytes, compr_size);
    // Get message from other side
    read_win_lin(sockfd, (uint8_t*)buff, compr_size);

    // Decrypt and authenticate the message from the server
    if (crypto_aead_read(&ctx_thm, (uint8_t*)compr, mac_thm, NULL, 0,(uint8_t*)buff, compr_size) == -1) 
    {
        /* If the message was altered during transmission*/
        exit_with_error(MESSAGE_ALTERED, "Last received message was altered, exiting"); 
    }

    // Decompress unencrypted text
    decompress_text((uint8_t*)compr, MAX, (uint8_t*)plain, compr_size);

    /* 
     Inserting terminator at the actual end 
     of string to avoid showing another garbage
    */
    for (int j = 0; j < TEXT_MAX; j++) {
        if(plain[j] == '\n' && j != TEXT_MAX-1) plain[j+1] = '\0';
    }
    printf("    From server: %s", plain);

    if (exiting("Server", plain) == 1) break; //Checks for stop-word

    crypto_wipe(plain, TEXT_MAX); //Clear plain

    }

}
///////////////////////////////////////////////////
///////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
///       Key exchange with x25519 + KDF with Blake2,          ///
/// inverse mapping of Elligator 2 and MAC side authentication ///
///               with long-term SK(PIN secured)               ///
//////////////////////////////////////////////////////////////////
static void key_exc_ell(int sockfd) 
{
 // Variables for key-exchange
 uint8_t your_sk[KEYSZ]; //our secret key
 uint8_t your_pk[KEYSZ]; //our public key
 uint8_t their_first_pk[KEYSZ]; //their first public key
 uint8_t their_second_pk[KEYSZ]; //their second public key
 uint8_t writing_key[KEYSZ]; //our writing key(their reading key)
 uint8_t reading_key[KEYSZ]; //our reading key(their writing key)
 uint8_t hidden[KEYSZ]; //our PK hidden with inverse mapping
 uint8_t plain_key[KEYSZ]; //key for authentication of your side
    
 // Variables for MAC of sides
 /*Keyed MAC of shared key(client), our authentication*/
 uint8_t mac_us[MACSZ]; 
 /*Keyed MAC of shared key(server), authentication of other side*/
 uint8_t mac_thm[MACSZ]; 
 int pad_size_mac = padme_size(MACSZ);
 uint8_t padded_mac_us[pad_size_mac]; //our padded MAC
 uint8_t padded_mac_thm[pad_size_mac]; //their padded MAC

 /*Computing size of padded hidden PK and creating variable*/
 int pad_size_key = padme_size(KEYSZ);
 uint8_t pad_your_pk[pad_size_key]; //our padded hidden PK
 uint8_t pad_hidden[pad_size_key]; //their padded hidden PK

 /*
 Generating first shared secret - our writing key, their reading key
 */

 /*Generate SK and hidden PK*/
 key_hidden(your_sk, your_pk, KEYSZ);

 // Padding of hidden PK
 pad_array(your_pk, pad_your_pk, KEYSZ, pad_size_key);

 // Sending/receiving PK(hidden and padded) (key exchange)
 write_win_lin(sockfd, pad_your_pk, pad_size_key);

 // Receiving PK(hidden and padded) (key exchange)
 read_win_lin(sockfd, pad_hidden, pad_size_key);

 /* 
 Return to the actual key-size and mapping scalar 
 to actual curve point(getting normal PK)
 */
 unpad_array(hidden, pad_hidden, KEYSZ);
 crypto_elligator_map(their_first_pk, hidden);

 // Compute our writing key(their reading key)
 kdf(writing_key, your_sk, their_first_pk, KEYSZ, CLIENT);

 // Asking and checking PIN for SK from user
 pin_checker(plain_key);

 // Compute keyed MAC of our writing key
 crypto_blake2b_keyed(mac_us, MACSZ, plain_key, KEYSZ, writing_key, KEYSZ);
    
 /*Padding our MAC*/
 pad_array(mac_us, padded_mac_us, MACSZ, pad_size_mac);
 /*Send padded MAC of our writing key(authentication of the sides)*/
 write_win_lin(sockfd, padded_mac_us, pad_size_mac);

 // Get padded MAC of their reading key(authentication of the sides)
 read_win_lin(sockfd, padded_mac_thm, pad_size_mac);
 /*Un-pad received MAC of other side*/
 unpad_array(mac_thm, padded_mac_thm, MACSZ); 

 // Checking if server is legit(if it owns shared SK)
 if (crypto_verify16(mac_us, mac_thm) == -1) {
     exit_with_error(UNEQUAL_MAC,"Other side isn`t legit, aborting");
 }
 
 /*
 Generating second shared secret - our reading key, their writing key
 */

 // Receiving PK(hidden and padded) (key exchange)
 read_win_lin(sockfd, pad_hidden, pad_size_key);

 /* 
 Return to the actual key-size and mapping scalar 
 to actual curve point(getting normal PK)
 */
 unpad_array(hidden, pad_hidden, KEYSZ);
 crypto_elligator_map(their_second_pk, hidden);

 // Compute our reading key(their writing key)
 kdf(reading_key, your_sk, their_second_pk, KEYSZ, CLIENT);

 // Compute keyed MAC of our reading key
 crypto_blake2b_keyed(mac_us, MACSZ, plain_key, KEYSZ, reading_key, KEYSZ);

 crypto_wipe(plain_key,KEYSZ); //wiping plain_key from memory
    
 /*Padding our MAC*/
 pad_array(mac_us, padded_mac_us, MACSZ, pad_size_mac);
 /*Send padded MAC of reading key(authentication of the sides)*/
 write_win_lin(sockfd, padded_mac_us, pad_size_mac);

 // Get padded MAC of their writing key(authentication of the sides)
 read_win_lin(sockfd, padded_mac_thm, pad_size_mac);
 /*Un-pad received MAC of other side*/
 unpad_array(mac_thm, padded_mac_thm, MACSZ); 

 // Checking if server is legit(if it owns shared SK)
 if (crypto_verify16(mac_us, mac_thm) == -1) {
     exit_with_error(UNEQUAL_MAC,"Other side isn`t legit, aborting");
 }
 
 /*Enterening "chatting" stage with derived shared keys*/
 chat(writing_key, reading_key, sockfd);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

////////////////////////////////
/// Main for socket creation ///
////////////////////////////////
int main(int argc, char *argv[]) 
{
 /*Copy default port in case user didn`t provided custom port*/
 int port = PORT; 
 char ip[IPSZ];
 /*Copy default IP in case user didn`t provided custom IP*/
 strcpy(ip,IP); 

 /*
 Next part of code handles arguments in main that provide user to 
 change default IP of server(loopback) and port
 */
 if (argc >= 2) { //Checking if arguments exsist
    if (argv[1][0] != '\0') { // Port/Help argument is not empty

        /*Print help*/
        if (strcmp(argv[1],"/h") == 0) help_print(CLIENT,PORT,IP,MAX);
            
        /*Changing var value to user-defined port*/
        port = atoi(argv[1]); 
        /*Checking port*/
        if (!(port > PORT_START && port <= PORT_END)) {
            exit_with_error(ERROR_PORT_INPUT,"Invalid port"); 
        }
    }   
    if (argc == 3 && argv[2][0] != '\0') { // IP argument is not empty       
            
        /*Checking IP for size*/
        if (strlen(argv[2]) >= IPSZ) {
            exit_with_error(ERROR_IP_INPUT,"Invalid IP(Too long)"); 
        }
            
        strcpy(ip,argv[2]); //Copy for checking format of entered IP
        ip_check(argv[2]); //Check of ip
        strcpy(ip,argv[2]); //Copy valid ip
        }
    }

 int sockfd = sockct_opn(port,ip);

 key_exc_ell(sockfd); //Entering "key exchange" stage

 sockct_cls(sockfd);

 printf("Program ended, press Enter:"); //End of program
 getchar();
 return 0;
}
////////////////////////////////
////////////////////////////////


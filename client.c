// Client-server api              //
// Client side                    //
// Version 0.2                    //
// Bachelor`s work project        //
// Technical University of Kosice //
// 01.09.2024                     //
// Nikita Kuropatkin              //
/*
This code provides encrypted client-server communication in form of chat.
Also this program has steganographic support in form of usage
Elligator 2 and PADME.
Program uses Monocypher`s library provided cryptographic primitives:
Crypto:
xChaCha20 - stream cypher for text encryption;
Blake2b - hash function used for key derivation;
X25519 - function for key exchange(uses Curve25519);
Crypto_wipe - function for constant time memory wipe.
Stegano:
Elligator 2 - algorithm that provides indistinguishability of public keys,
that will be used in key exchange;
PADME - padding algorithm.
This code is client side,
!REMEMBER! SERVER MUST BE RUNNED FIRST!
*/

//////////Version history//////////
/*
Version 0.3 : 
# Migration of shared functions to shared.c;
# Correction of macros;
# Structured error returning code;
# Corrected problem with memory in number generation;
Version 0.2 :
# Added comments, explanations etc. ;
# Corrected mistake for generating random numbers in Windows ;
# Functions that needed different code on different platforms created(main code cleared from ifdef) ;
# Warnings cleared ;
# PADME padding with random numbers now(Mistake corrected) ;
# Dealing with buffer overflow added ;
Version 0.1 - basic functionality;
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "monocypher.h"
#include "shared.h"


#define MAX 400    //message size, can be changed at your preference

#define KEYSZ 32   //SK, PK, Hidden PK sizes
#define NONSZ 24   //Nonce size
#define IP "127.0.0.1" //Ip address of the server
/*
Changing this sizes can and will create security risks or program instability!
If you need different key or nonce sizes, pls read whole code before
and consider using different functions from Monocypher
*/
#define PORT 8087 // port number
#define SA struct sockaddr



//////////////////////////////////////////
/// Socket opener ///
//////////////////////////////////////////
/*
This function purpose is to open sockets for WIN and LIN OS
*/
int sockct_opn(){
    int sockfd;
    struct sockaddr_in servaddr;
        #ifdef _WIN32
            WSADATA wsaData;
            int iResult;
            iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (iResult != 0) {
                printf("WSAStartup failed: %d\n", iResult);
                exit(1);
            }
        #endif

    // Socket creation and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
            #ifdef _WIN32
                WSACleanup();
            #endif
        exit(2);
    } else {
        printf("Socket successfully created..\n");
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(IP);	//loopback IP
    servaddr.sin_port = htons(PORT);

    // Connect the client socket to the server socket
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection with the server failed...\n");
            #ifdef _WIN32
                closesocket(sockfd);
                WSACleanup();
            #else
                close(sockfd);
            #endif
        exit(10);
    } else {
        printf("Connected to the server..\n");
    }
    return sockfd;

}


///////////////////////////////////////////////////
/// Client-server communication "chatting" ///
///////////////////////////////////////////////////
void chat(uint8_t* secret,int sockfd){
    // Creating variables
    char buff[MAX]; //buffer for encrypted text
    char plain[MAX]; // buffer for decrypted text
    uint8_t nonce[NONSZ]; //our nonce array
    uint8_t nonce_thm[NONSZ]; //their nonce array
    int pad_size = padme_size(sizeof(nonce)); // size of padded nonce
    uint8_t pad_nonce[pad_size]; //new array that will contain padded nonce
    uint64_t blkcnt_us = 0; //block counter for us(chacha enc)
    uint64_t blkcnt_them = 0; //their block counter (chacha enc)

    // Generate nonce
        random_num(nonce, NONSZ);
    // Pad Nonce
        pad_array(nonce, pad_nonce,sizeof(nonce), pad_size);
    // Send/recieve nonce
        write_win_lin(sockfd, pad_nonce, sizeof(pad_nonce));

        read_win_lin(sockfd, pad_nonce, sizeof(pad_nonce));
    //Un-pad Nonce
        unpad_array(nonce_thm, pad_nonce,sizeof(nonce));

     while (1) {

        memset(buff, 0, MAX);
        memset(plain, 0, MAX);

        // Recieve message to send
        printf("To server: ");
        if (fgets(plain, sizeof(plain), stdin) == NULL) {
            printf("Error reading input. Program is stopped!");
            exit(6);
        }
        // Buffer overflow, clear stdin
        if (plain[strlen(plain) - 1] != '\n') {
            printf("Your message was too long, boundaries is: %d symbols, only those will be sent.\n",MAX);
            clear();
            plain[strlen(plain) - 1] = '\n';
            plain[MAX - 1] = '\0';
        }

        // Send block counter
        write_win_lin(sockfd, (uint8_t*)&blkcnt_us, sizeof(blkcnt_us));
        // Encrypt message
        blkcnt_us = crypto_chacha20_x((uint8_t*)buff, (uint8_t*)plain, strlen(plain), secret, nonce, blkcnt_us);


        // Send  and encrypted message to server

        write_win_lin(sockfd, (uint8_t*)buff, sizeof(buff));

		if (strncmp(plain, "exit", 4) == 0) {
            printf("Client Exit...\n");
            break;
        }

        // Clear the buffer and receive the response
        memset(buff, 0, sizeof(buff));
        crypto_wipe(plain, MAX);// clear plain


        read_win_lin(sockfd, (uint8_t*)&blkcnt_them, sizeof(blkcnt_them));
        read_win_lin(sockfd, (uint8_t*)buff, sizeof(buff));

        // Decrypt the message from the server
        crypto_chacha20_x((uint8_t*)plain, (uint8_t*)buff, sizeof(buff), secret, nonce_thm, blkcnt_them);

        // Print the server's response
        for(int j = 0; j < MAX; j++) {
            if(plain[j] == '\n' && j != MAX-1) plain[j+1] = '\0';
        }
        printf("    From Server: %s", plain);

        // Exit the loop if the message is "exit"
        if (strncmp(plain, "exit", 4) == 0) {
            printf("Client Exit...\n");
            break;
        }

        crypto_wipe(plain, MAX);// clear plain

    }

}
///////////////////////////////////////////////////
///////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
/// Key exchange with x25519 + KDF with Blake2, inverse mapping of elligator  ///
////////////////////////////////////////////////////////////////////////////////////////
void key_exc_ell(int sockfd) {
    // Variables for key-exchange
    uint8_t your_sk[KEYSZ]; //your secret key
    uint8_t your_pk[KEYSZ]; //your private key
    uint8_t their_pk[KEYSZ]; //their private key
    uint8_t shared_secret[KEYSZ]; 
    uint8_t shared_keys[KEYSZ]; //shared key material
    uint8_t tweak; // Elligator`s tweak
    uint8_t hidden[KEYSZ]; //your PK hiddent with inverse mapping

    // Computing size of padded hidden PK and creating variable
    int pad_size = padme_size(sizeof(your_pk));
    uint8_t pad_your_pk[pad_size]; //your padded hidden PK
    uint8_t pad_hidden[pad_size]; //their padded hidden PK

    // Tweak for elligator`s inverse map
    random_num(&tweak, 1);
    // Inverse mapping(mapping Curve point to a scalar)
    while(1) {
        random_num(your_sk, KEYSZ);
        crypto_x25519_dirty_small(your_pk, your_sk);
        if (crypto_elligator_rev(your_pk, your_pk, tweak) == 0)
            break;
    }

    // Padding of hidden PK
    pad_array(your_pk, pad_your_pk,sizeof(your_pk), pad_size);

    // Sending/receiving PK(hidden and padded) (key exchange)
    write_win_lin(sockfd, pad_your_pk, sizeof(pad_your_pk));

    // Receiving PK(hidden and padded) (key exchange)
    read_win_lin(sockfd, pad_hidden, sizeof(pad_hidden));


    // Return to the actual key-size and mapping scalar to actual curve point(getting normal PK)
    unpad_array(hidden, pad_hidden,sizeof(hidden));
    crypto_elligator_map(their_pk, hidden);

    // Compute the shared secret
    crypto_x25519(shared_secret, your_sk, their_pk);

    // Computing PK without inverse mapping 4 KDF
    crypto_x25519_dirty_small(your_pk, your_sk);
    // Derive shared keys(KDF with Blake2)
    crypto_blake2b_ctx ctx;
    crypto_blake2b_init(&ctx, KEYSZ);
    crypto_blake2b_update(&ctx, shared_secret, KEYSZ);
    crypto_blake2b_update(&ctx, your_pk, KEYSZ);
    crypto_blake2b_update(&ctx, their_pk, KEYSZ);
    crypto_blake2b_final(&ctx, shared_keys);

    // Shared key for encryption
    uint8_t *key_1 = shared_keys;

    //Calling function for encrypted communication

    chat(key_1,sockfd);

    // Clean up
    crypto_wipe(shared_secret, KEYSZ);
    crypto_wipe(shared_keys, KEYSZ);
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////
/// Main for socket creation ///
///////////////////////////////////
int main(int argc, char *argv[]) {
    /*arguments in main providing user to change default IP of server(loopback) and port*/
    if (argc >= 2) {//checking if arguments exsist
        if(argv[1][0] != '\0'){ // IP argument is not empty
        #undef IP
        #define IP argv[1] //redefining macro to users ip
        }
        if(argv[2][0] != '\0'){ // Port argument is not empty
        #undef PORT
        #define PORT atoi(argv[2]) //redefining macro to users port
        } 
    }

    int sockfd = sockct_opn();
    
    key_exc_ell(sockfd);

    sockct_cls(sockfd);

    return 0;
}


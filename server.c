// Client-server api              //
// Server side                    //
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
This code is server side,
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
#include <math.h>
#include "monocypher.h"
#include "shared.h"
#include "error.h"
#define MAX 400     //message size, can be changed at your preference
//This size means amount of characters that will be readed from stdin to send it
#define KEYSZ 32   //SK, PK, Hidden PK sizes
#define NONSZ 24   //Nonce size
/*
Changing this sizes can and will create security risks or program instability!
If you need different key or nonce sizes, pls read whole code before
and consider using different functions from Monocypher
*/
#define PORT 8087 // port number(modify on both sides!)
#define EXIT "exit" // stop word, if someone use it in conversation, it will end


#define SA struct sockaddr
#ifdef _WIN32// defining macro for address length(using different types in WIN and LINUX)
    #define LEN int 
#else
    #define LEN socklen_t
#endif 
#define SERVER 0 //Macro for KDF (do not change this macro for this side) 

//////////////////////////////////////////
/// Socket opener ///
//////////////////////////////////////////
/*
This function purpose is to open sockets for WIN and LIN OS
*/
int sockct_opn(int *sockfd,int port){
    // initialization for sockets win
    init_sock();
 
    int connfd;
    struct sockaddr_in servaddr, cli;

    // Create socket
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sock_check(*sockfd);//check socket
    
    memset(&servaddr, 0, sizeof(servaddr));

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(port == 0)servaddr.sin_port = htons(PORT); //checking if user entered custom port
    else servaddr.sin_port = htons(port);
    // Bind the socket
    if (bind(*sockfd, (SA*)&servaddr, sizeof(servaddr)) == -1){//checking for error
        sockct_cls(*sockfd);
        exit_with_error(ERROR_BINDING, "Socket bind failed");
    }
    else{
        printf("Socket successfully binded..\n");
    }
    // Listen for incoming connections
    if (listen(*sockfd, 5) == -1){ //checking if listening failed
        sockct_cls(*sockfd);
        exit_with_error(ERROR_SOCKET_LISTENING, "Listen failed");
    }
    else{
        printf("Server listening..\n");
    }
    LEN len = sizeof(cli); //addr_len using macro(diff types for WIN & LINUX)

    // Accept a connection
    connfd = accept(*sockfd, (SA*)&cli, &len);
    if(connfd == -1){//check for error 
        sockct_cls(*sockfd);
        exit_with_error(ERROR_SERVER_ACCEPT, "Server accept failed");
    }
    else{
        printf("Server accept the client...\n");
    }
    return connfd;

}


///////////////////////////////////////////////////
/// Client-server communication "chatting" ///
///////////////////////////////////////////////////
void chat(uint8_t* secret,int sockfd)
{
    //Variables for encrypted communication+padding Nonce
    char buff[MAX]; //buffer for encrypted text
    char plain[MAX]; // buffer for decrypted text
    uint8_t nonce[NONSZ]; //our nonce array
    uint8_t nonce_thm[NONSZ]; //their nonce array
    int pad_size = padme_size(sizeof(nonce)); // size of padded nonce
    uint8_t pad_nonce[pad_size]; //new array that will contain padded nonce
    uint8_t pad_nonce_their[pad_size]; //new array that will contain padded nonce
    uint64_t blkcnt_us = 0; //block counter for us(chacha enc)
    uint64_t blkcnt_them = 0; //their block counter (chacha enc)

    // Generate nonce
        random_num(nonce, NONSZ);
    // Pad Nonce
        pad_array(nonce, pad_nonce,sizeof(nonce), pad_size);
    // Send/recieve nonce
        read_win_lin(sockfd, pad_nonce_their, sizeof(pad_nonce));

        write_win_lin(sockfd, pad_nonce, sizeof(pad_nonce));
    //Un-pad Nonce
        unpad_array(nonce_thm, pad_nonce_their,sizeof(nonce));

    while (1) {

        //Clearing vars
        memset(buff, 0, MAX);
        memset(plain, 0, MAX);


        //Read blck counter
        read_win_lin(sockfd, (uint8_t*)&blkcnt_them, sizeof(blkcnt_them));
        //Read buff
        read_win_lin(sockfd, (uint8_t*)buff, sizeof(buff));

        // Decrypt message from client
        crypto_chacha20_x((uint8_t*)plain, (uint8_t*)buff, sizeof(buff), secret, nonce_thm, blkcnt_them );

        //inserting terminator at the actual end of string to avoid showing another garbage
        for(int j = 0; j < MAX; j++) {
            if(plain[j] == '\n' && j != MAX-1) plain[j+1] = '\0';
        }

        // Print decrypted message
        printf("    From client: %s", plain);


        // Check for exit command
        if (strncmp(EXIT, plain, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }

        // Clear buffers
        memset(buff, 0, MAX);
        crypto_wipe(plain, MAX);// clear plain

        // Read server message from stdin
        printf("To client: ");
        
        if (fgets(plain, sizeof(plain), stdin) == NULL) {
            exit_with_error(ERROR_GETTING_INPUT, "Error reading input");
        }
        // Buffer overflow, clear stdin
        if (plain[strlen(plain) - 1] != '\n') {
            printf("Your message was too long, boundaries is: %d symbols, only those will be sent.\n",MAX);
            clear();
            plain[strlen(plain) - 1] = '\n';
        }

        // Send block counter
        write_win_lin(sockfd, (uint8_t*)&blkcnt_us, sizeof(blkcnt_us));
        // Encrypt
        blkcnt_us = crypto_chacha20_x((uint8_t*)buff, (uint8_t*)plain, strlen(plain), secret, nonce , blkcnt_us);

         // Send encrypted message to server
        write_win_lin(sockfd, (uint8_t*)buff, sizeof(buff));

        // Check for exit command (again, to exit the loop)
        if (strncmp(EXIT, plain, 4) == 0) {
            printf("Server Exit...\n");
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
    // Creating variables
    uint8_t your_sk[KEYSZ]; //your secret key
    uint8_t your_pk[KEYSZ]; //your private key
    uint8_t their_pk[KEYSZ]; //their private key
    uint8_t shared_key[KEYSZ]; //shared key material
    uint8_t hidden[KEYSZ]; //your PK hiddent with inverse mapping

    // Computing size of padded hidden PK and creating variable
    int pad_size = padme_size(sizeof(hidden));
    uint8_t pad_your_pk[pad_size]; //your padded hidden PK
    uint8_t pad_hidden[pad_size]; //their padded hidden PK

    // Receive their hidden and padded PK
    read_win_lin(sockfd, pad_hidden, sizeof(pad_hidden));

    //Un-pad received hidden PK + map the scalar(a.k.a hidden PK) to real point on a curve
    unpad_array(hidden, pad_hidden,sizeof(hidden));
    crypto_elligator_map(their_pk, hidden);

    //generate SK and hidden PK
    key_hidden(your_sk, your_pk, KEYSZ);

    // padding hidden PK
    pad_array(your_pk, pad_your_pk,sizeof(your_pk), pad_size);

    // Send padded and hidden PK to client
    write_win_lin(sockfd, pad_your_pk, sizeof(pad_your_pk));

    // Compute shared secret
    kdf(shared_key, your_sk, their_pk, KEYSZ, SERVER);

    // Main chat loop
    chat(shared_key,sockfd);

    crypto_wipe(shared_key, KEYSZ);
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////
/// Main for socket creation ///
///////////////////////////////////
int main(int argc, char *argv[]) {
    int port = 0;
    /*arguments in main providing user to change default Port*/
    if (argc == 2 && argv[1][0] != '\0') {//checking if arguments exsist & not empty
        port =  atoi(argv[1]); //redefining var to users port
    }
    int sockfd;
    int connfd = sockct_opn(&sockfd, port);
    // Function for chat
    key_exc_ell(connfd);

    sockct_cls(sockfd);

    return 0;
}

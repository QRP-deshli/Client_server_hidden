// Client-server api              //
// Client side                    //
// Version 0.5.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 10.11.2024                     //
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

/*
Links:
https://elligator.org/
https://monocypher.org/
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
# Added posibility to change default port and IP of server, while running program
# All functions for compatibility with different OSs are contained in shared.c now
# Added comments more comments and Macros
# Added functions kdf() and key_hidden(), also additional macros for them ,
kdf() - key derivation with blake2b; key_hidden() for masking Public keys
# Added new logic for printing errors, check config.c and error.h for further explanation
# Corrected mistake with shared key
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
#include "network.h"
#include "addition.h"
#include "random.h"
#include "crypto.h"
#include "parameters.h"
#include "secret.h"

#define IPSZ 16   //IP size
#define IP "127.0.0.1" //Ip address of the server

#define CLIENT 1 //Macro for KDF (do not change this macro for this side) 
#define SA struct sockaddr

//////////////////////////////////////////
/// Socket opener ///
//////////////////////////////////////////
/*
This function purpose is to open sockets for WIN and LIN OS
Return value of this function is file descriptor of socket(ID of socket)
Also input variables are port number and server IP
*/
int sockct_opn(int port, char *ip){
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
    char buff[MAX]; // Buffer for encrypted text
    char plain[MAX]; // Buffer for decrypted text

    // Variables for nonce
    uint8_t nonce_us[NONSZ]; // Our nonce array
    uint8_t nonce_thm[NONSZ]; // Their nonce array
    int pad_size = padme_size(NONSZ); // Size of padded nonce
    uint8_t pad_nonce[pad_size]; // New array that will contain padded nonce
    uint8_t pad_nonce_their[pad_size]; // New array that will contain their padded nonce

    // Variables for MAC
    uint8_t mac_us[MACSZ]; // MAC of our messages
    uint8_t mac_thm[MACSZ]; // MAC of their messages
    pad_size = padme_size(MACSZ);
    uint8_t padded_mac_us[pad_size]; // Our padded MAC
    uint8_t padded_mac_thm[pad_size]; // Their padded MAC

    //AEAD state variables:
    crypto_aead_ctx ctx_us;// Our structure for aead(stores and increments Shared Key, Nonce and block counter)
    crypto_aead_ctx ctx_thm;// Their structure for aead(stores and increments Shared Key, Nonce and block counter)

    // Generate nonce
    random_num(nonce_us, NONSZ);

    // Pad Nonce
    pad_array(nonce_us, pad_nonce,sizeof(nonce_us), pad_size);

    // Send/recieve nonce
    write_win_lin(sockfd, pad_nonce, sizeof(pad_nonce));
    read_win_lin(sockfd, pad_nonce_their, sizeof(pad_nonce_their));

    // Un-pad Nonce
    unpad_array(nonce_thm, pad_nonce_their,sizeof(nonce_thm));

    // Initialization of an AEAD states:
    crypto_aead_init_x(&ctx_us, secret, nonce_us);
    crypto_aead_init_x(&ctx_thm, secret, nonce_thm);

     while (1) {
        // Clear buffers 
        memset(buff, 0, MAX);
        memset(plain, 0, MAX);

        // Recieve message to send
        printf("To server: ");
        if (fgets(plain, sizeof(plain), stdin) == NULL) {
            exit_with_error(ERROR_GETTING_INPUT, "Error reading input");
        }

        // Buffer overflow, clear stdin
        if (plain[strlen(plain) - 1] != '\n') {
            printf("Your message was too long, boundaries is: %d symbols, only those will be sent.\n",MAX);
            clear();
            plain[strlen(plain) - 1] = '\n';
        }

        // Encrypt message and generate MAC for it
        crypto_aead_write(&ctx_us, (uint8_t*)buff, mac_us,NULL, 0,(uint8_t*)plain, MAX);

        // Send padded MAC of our message
        pad_array(mac_us, padded_mac_us, sizeof(mac_us), sizeof(padded_mac_us));// padding our MAC
        write_win_lin(sockfd, padded_mac_us, sizeof(padded_mac_us));
        
        // Send encrypted message to server
        write_win_lin(sockfd, (uint8_t*)buff, sizeof(buff));

        if(exiting("Client", plain) == 1)break; //checks for stop-word
		
        // Clear buffers 
        memset(buff, 0, MAX);
        crypto_wipe(plain, MAX);// clear plain

        // Get padded MAC of message
        read_win_lin(sockfd, padded_mac_thm, sizeof(padded_mac_thm));
        unpad_array(mac_thm, padded_mac_thm, sizeof(mac_thm)); //Un-pad recieved MAC

        // Get message from other side
        read_win_lin(sockfd, (uint8_t*)buff, sizeof(buff));

        // Decrypt and authenticate the message from the server
        if (crypto_aead_read(&ctx_thm, (uint8_t*)plain, mac_thm, NULL, 0,(uint8_t*)buff, MAX) == -1) {
            exit_with_error(MESSAGE_ALTERED, "Last received message was altered, exiting"); // if the message was altered during transmission
        }

        // Inserting terminator at the actual end of string to avoid showing another garbage
        for(int j = 0; j < MAX; j++) {
            if(plain[j] == '\n' && j != MAX-1) plain[j+1] = '\0';
        }
        printf("    From Server: %s", plain);

        if(exiting("Server", plain) == 1)break; //checks for stop-word

        crypto_wipe(plain, MAX);// clear plain

    }

}
///////////////////////////////////////////////////
///////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
/// Key exchange with x25519 + KDF with Blake2, inverse mapping of elligator         ///
////////////////////////////////////////////////////////////////////////////////////////
void key_exc_ell(int sockfd) {
    // Variables for key-exchange
    uint8_t your_sk[KEYSZ]; //your secret key
    uint8_t your_pk[KEYSZ]; //your private key
    uint8_t their_pk[KEYSZ]; //their private key
    uint8_t shared_key[KEYSZ]; //shared key material
    uint8_t hidden[KEYSZ]; //your PK hiddent with inverse mapping
    
    // Variables for MAC of sides
    uint8_t mac_us[MACSZ]; //keyed MAC of shared key(client), our authentication 
    uint8_t mac_thm[MACSZ]; //keyed MAC of shared key(server), authentication of other side
    int pad_size = padme_size(MACSZ);
    uint8_t padded_mac_us[pad_size]; //our padded MAC
    uint8_t padded_mac_thm[pad_size]; //their padded MAC

    // Computing size of padded hidden PK and creating variable
    pad_size = padme_size(KEYSZ);
    uint8_t pad_your_pk[pad_size]; //your padded hidden PK
    uint8_t pad_hidden[pad_size]; //their padded hidden PK

    //generate SK and hidden PK
    key_hidden(your_sk, your_pk, KEYSZ);

    // Padding of hidden PK
    pad_array(your_pk, pad_your_pk, sizeof(your_pk), sizeof(pad_your_pk));

    // Sending/receiving PK(hidden and padded) (key exchange)
    write_win_lin(sockfd, pad_your_pk, sizeof(pad_your_pk));

    // Receiving PK(hidden and padded) (key exchange)
    read_win_lin(sockfd, pad_hidden, sizeof(pad_hidden));

    // Return to the actual key-size and mapping scalar to actual curve point(getting normal PK)
    unpad_array(hidden, pad_hidden,sizeof(hidden));
    crypto_elligator_map(their_pk, hidden);

    // Compute the shared secret
    kdf(shared_key, your_sk, their_pk, KEYSZ, CLIENT);

    // Compute keyed MAC of our shared key
    crypto_blake2b_keyed(mac_us, sizeof(mac_us), key_original, sizeof(key_original), shared_key, sizeof(shared_key));
    
    // Send padded MAC of shared key(authentication of the sides)
    pad_array(mac_us, padded_mac_us,sizeof(mac_us), sizeof(padded_mac_us));// padding our MAC
    write_win_lin(sockfd, padded_mac_us, sizeof(padded_mac_us));

    // Get padded MAC of shared key(authentication of the sides)
    read_win_lin(sockfd, padded_mac_thm, sizeof(padded_mac_thm));
    unpad_array(mac_thm, padded_mac_thm, sizeof(mac_thm)); //Un-pad received MAC of other side

    // Checking if server is legit(if it owns shared SK)
    if(crypto_verify16(mac_us, mac_thm) == -1){
        exit_with_error(UNEQUAL_MAC,"Other side isn`t legit, aborting");
    }
    
    chat(shared_key,sockfd);

    crypto_wipe(shared_key, KEYSZ);
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////
/// Main for socket creation ///
///////////////////////////////////
int main(int argc, char *argv[]) {
    int port = PORT; 
    char ip[IPSZ];
    strcpy(ip,IP);
    /*arguments in main providing user to change default IP of server(loopback) and port*/
    if (argc >= 2) {//checking if arguments exsist
        if(argv[1][0] != '\0'){ // Port argument is not empty
            if(strcmp(argv[1],"/h") == 0)help_print(CLIENT,PORT,IP,MAX);//print help
            port =  atoi(argv[1]); //redefining var to users port
            if(!(port > 1024 && port <= 65535))exit_with_error(ERROR_PORT_INPUT,"Invalid port");//Checking port
        }
        if(argc == 3 && argv[2][0] != '\0'){ // IP argument is not empty
            if(strlen(argv[2]) >= IPSZ)exit_with_error(ERROR_IP_INPUT,"Invalid IP(Too long)");//Checking IP
            strcpy(ip,argv[2]); //copy for checking 
            ip_check(argv[2]); //check of ip
            strcpy(ip,argv[2]); //copy valid ip
        }
    }

    int sockfd = sockct_opn(port,ip);

    key_exc_ell(sockfd);

    sockct_cls(sockfd);

    printf("Program ended, press Enter:");
    getchar();
    return 0;
}


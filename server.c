// Client-server api              //
// Client side                    //
// Version 0.5                    //
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
This code is server side,
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
#include <math.h>
#include "monocypher.h"
#include "network.h"
#include "addition.h"
#include "random.h"
#include "crypto.h"
#include "error.h"
#include "parameters.h"
#include "secret.h"

#define SA struct sockaddr
#define SERVER 0 //Macro for KDF (do not change this macro for this side) 

//////////////////////////////////////////
/// Socket opener ///
//////////////////////////////////////////
/*
This function purpose is to open sockets for WIN and LIN OS
Return value of this function is connection file descriptor (ID of connection)
Also input variables are port number and server IP
*/
int sockct_opn(int *sockfd,int port){
    // initialization for sockets win
    init_sock();
 
    // create variables
    int connfd;
    struct sockaddr_in servaddr, cli;

    // Create socket
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sock_check(*sockfd);//check socket
    
    memset(&servaddr, 0, sizeof(servaddr));

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
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
    uint8_t nonce_us[NONSZ]; //our nonce array
    uint8_t nonce_thm[NONSZ]; //their nonce array
    int pad_size = padme_size(sizeof(nonce_us)); // size of padded nonce
    uint8_t pad_nonce[pad_size]; //new array that will contain padded nonce
    uint8_t pad_nonce_their[pad_size]; //new array that will contain their padded nonce
    uint8_t mac_us[MACSZ]; //MAC of our messages
    uint8_t mac_thm[MACSZ]; //MAC of their messages
    //AEAD state variables:
    crypto_aead_ctx ctx_us;//our structure for aead(stores and increments Shared Key, Nonce and block counter)
    crypto_aead_ctx ctx_thm;//their structure for aead(stores and increments Shared Key, Nonce and block counter)

    // Generate nonce
        random_num(nonce_us, NONSZ);

    // Pad Nonce
        pad_array(nonce_us, pad_nonce,sizeof(nonce_us), pad_size);

    // Send/recieve nonce
        read_win_lin(sockfd, pad_nonce_their, sizeof(pad_nonce_their));
        write_win_lin(sockfd, pad_nonce, sizeof(pad_nonce));

    //Un-pad Nonce
        unpad_array(nonce_thm, pad_nonce_their,sizeof(nonce_thm));

    //Initialization of an AEAD states:
        crypto_aead_init_x(&ctx_us, secret, nonce_us);
        crypto_aead_init_x(&ctx_thm, secret, nonce_thm);    
    while (1) {

        //Clearing vars
        memset(buff, 0, MAX);
        memset(plain, 0, MAX);

        // Receive messages from client(MAC and encrypted text)
        read_win_lin(sockfd, (uint8_t*)&mac_thm, sizeof(mac_thm));
        read_win_lin(sockfd, (uint8_t*)buff, sizeof(buff));

        // Decrypt and authenticate the message from the server
        if (crypto_aead_read(&ctx_thm, (uint8_t*)plain, mac_thm, NULL, 0,(uint8_t*)buff, MAX) == -1) {
            exit_with_error(MESSAGE_ALTERED, "Last received message was altered, exiting"); // if the message was altered during transmission
        }

        //Insert terminator at the actual end of string to avoid showing another garbage
        for(int j = 0; j < MAX; j++) {
            if(plain[j] == '\n' && j != MAX-1) plain[j+1] = '\0';
        }

        // Print decrypted message
        printf("    From client: %s", plain);
        
        //Check for stop-word
        if(exiting("Client", plain) == 1)break; 

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
            clear();// clearing stdin
            plain[strlen(plain) - 1] = '\n';
        }

        // Encrypt message and generate MAC for it
        crypto_aead_write(&ctx_us, (uint8_t*)buff, mac_us,NULL, 0,(uint8_t*)plain, MAX);
        
        // Send MAC for our message to server
        write_win_lin(sockfd, (uint8_t*)&mac_us, sizeof(mac_us));
        
        // Send encrypted message to server
        write_win_lin(sockfd, (uint8_t*)buff, sizeof(buff));

        // Check for exit command (again, to exit the loop)
        if(exiting("Server", plain) == 1)break; //checks for stop-word

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
    uint8_t mac_us[MACSZ]; //keyed MAC of shared key(client), our authentication 
    uint8_t mac_thm[MACSZ]; //keyed MAC of shared key(server), authentication of other side

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

    // Compute keyed MAC of our shared key
    crypto_blake2b_keyed(mac_us, sizeof(mac_us), key_original, sizeof(key_original), shared_key, sizeof(shared_key));

    // Get and send MAC of shared(authentication of the sides)
    read_win_lin(sockfd, mac_thm, sizeof(mac_thm));
    write_win_lin(sockfd, mac_us, sizeof(mac_us));

    // Checking if server is legit(if it owns shared SK)
    if(crypto_verify16(mac_us, mac_thm) == -1){
        exit_with_error(UNEQUAL_MAC,"Other side isn`t legit, aborting");
    }

    // Main chat loop
    chat(shared_key,sockfd);

    // Clearing shared key
    crypto_wipe(shared_key, KEYSZ);
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////
/// Main for socket creation ///
///////////////////////////////////
int main(int argc, char *argv[]) {
    int port = PORT;

    /*arguments in main providing user to change default Port*/
    if (argc == 2 && argv[1][0] != '\0') {//checking if arguments exsist & not empty
        if(strcmp(argv[1],"/h") == 0)help_print(SERVER,PORT,NULL,MAX);//print help
        port =  atoi(argv[1]); //redefining var to users port
        if(!(port > 1024 && port <= 65535))exit_with_error(ERROR_PORT_INPUT,"Invalid port");//Checking port
    }

    int sockfd;
    int connfd = sockct_opn(&sockfd, port);

    // Function for chat
    key_exc_ell(connfd);

    sockct_cls(sockfd);

    printf("Program ended, press Enter:");
    getchar();
    return 0;
}

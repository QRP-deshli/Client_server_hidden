#include"shared.h"

//////////////////////////////////////////
/// Clearing input   ///
//////////////////////////////////////////
void clear (){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


//////////////////////////////////////////
/// Data sender ///
//////////////////////////////////////////
/*
This function purpose is to send data over open sockets for WIN and LIN OS
*/
void read_win_lin(int sockfd, uint8_t *msg, unsigned int size){
    #ifdef _WIN32
        if (recv(sockfd, (char*)msg, size, 0) == SOCKET_ERROR) {
            exit_with_error(ERROR_RECEIVING_DATA, "Recieving failed");
         }
    #else
        if (read(sockfd, msg, size) == -1) {
            exit_with_error(ERROR_RECEIVING_DATA, "Recieving failed");
        }
    #endif

}
//////////////////////////////////////////
/// Data receiver ///
//////////////////////////////////////////
/*
This function purpose is to receive data over open sockets for WIN and LIN OS
*/
void write_win_lin(int sockfd, uint8_t *msg, unsigned int size){
    #ifdef _WIN32
        if (send(sockfd, (char*)msg, size, 0) == SOCKET_ERROR) {
            exit_with_error(ERROR_SENDING_DATA, "Writing failed");
         }
    #else
         if (write(sockfd, msg, size) == -1) {
            exit_with_error(ERROR_SENDING_DATA, "Writing failed");
        }
    #endif

}
//////////////////////////////////////////
//////////////////////////////////////////


/////////////////
/// PADME ///
/////////////////
int padme_size(int L) {
    //modified PADME for padding of the key + nonce
    int E = (int)log2(L);
    int S = (int)log2(E) + 1;
    int lastBits = E - S;
    int bitMask = (1 << lastBits) - 1;
    return (L + bitMask);
}

//Copy the OG PK to a Padded array, or turn back OG size to a padded PK
void pad_array(uint8_t* array, uint8_t* pad_array, int og_size, int new_size) {

  memcpy(pad_array, array, og_size);

  if (new_size > og_size)random_num(&pad_array[og_size], new_size-og_size);
}
void unpad_array(uint8_t* array, uint8_t* pad_array, int og_size) {
    memcpy(array, pad_array,og_size);
}
/////////////////
/////////////////


//////////////////////////////////////////
/// Socket closer///
//////////////////////////////////////////
/*
This function purpose is to close existing sockets for WIN and LIN OS
*/
void sockct_cls(int sockfd){
        // Close the socket
        #ifdef _WIN32
            closesocket(sockfd);
            WSACleanup();
        #else
            close(sockfd);
        #endif
}
///////////////////////////////////////////////////
///////////////////////////////////////////////////

///////////////////////////////
/// Socket initializer///
//////////////////////////////
/*
This function purpose is to initialize socket for windows
*/
void init_sock(){
    #ifdef _WIN32
        WSADATA wsaData;
        int iResult;
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            exit_with_error(ERROR_WSA, "WSAStartup failed");
        }
    #endif
}
//////////////////////////////
/////////////////////////////

///////////////////////////////
/// Socket creation checker///
//////////////////////////////
/*
This function purpose is to initialize socket for windows
*/
void sock_check(int sockfd){
    if (sockfd == -1) {
        exit_with_error(ERROR_SOCKET_CREATION, "Socket creation failed");
            #ifdef _WIN32
                WSACleanup();
            #endif
    } else {
        printf("Socket successfully created..\n");
    }
}
//////////////////////////////
//////////////////////////////

///////////////////////////////
/// KDF with Blake2b///
//////////////////////////////
/*
This function purpose is derive shared key from 
raw shared key and PKs of both sides.
The last parameter is a switch to use specific 
order for a specific side(server and client)
*/
void kdf(uint8_t *shared_key, uint8_t *your_sk, uint8_t *their_pk, int keysz, int side){
    uint8_t shared_secret[keysz]; //raw shared key
    uint8_t your_pk[keysz]; //your PK
    // Compute PK(again) but without inverse mapping
    crypto_x25519_dirty_small(your_pk, your_sk);
        
    // Compute shared secret
    crypto_x25519(shared_secret, your_sk, their_pk);

    // KDF with blake2
    crypto_blake2b_ctx ctx;
    crypto_blake2b_init(&ctx, keysz);
    crypto_blake2b_update(&ctx, shared_secret, keysz);
    if(side == 1){
    crypto_blake2b_update(&ctx, your_pk, keysz);
    crypto_blake2b_update(&ctx, their_pk, keysz);
    }
    else{
        crypto_blake2b_update(&ctx, their_pk, keysz);
        crypto_blake2b_update(&ctx, your_pk, keysz);
    }
    crypto_blake2b_final(&ctx, shared_key);
    // Shared key for encryption
        
    //cleaning
    crypto_wipe(shared_secret, keysz);
}
///////////////////////////////
///////////////////////////////

/////////////////////////////////////////
/// Function for generating hidden PKs///
/////////////////////////////////////////
/*This function takes two empty arrays for keys
generates tweak for Elligator, next in infinite cycle
it generates SK and derives PK from it. If that PK
can be mapped to a random string with Elligator 2 cycle ends,
 if not it continues and new SKs and PKs are generated
 */
void key_hidden(uint8_t *your_sk, uint8_t *your_pk, int keysz){
    uint8_t tweak; // Elligator`s tweak
    // Tweak for elligator`s inverse map
    random_num(&tweak, 1);
    //Function for creating SK and computing PK, then inverse mapping it to a scalar
    while (1) {
    random_num(your_sk, keysz);
    crypto_x25519_dirty_small(your_pk, your_sk);
    if (crypto_elligator_rev(your_pk, your_pk, tweak) == 0)
        break;
    }
}
/////////////////////////////////////////
/////////////////////////////////////////
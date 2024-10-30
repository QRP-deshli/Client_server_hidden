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
            printf("Recieving failed with error\n");
            exit(7);
         }
    #else
        if (read(sockfd, msg, size) == -1) {
            perror("Recieving failed with error");
            exit(7);
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
            printf("Writing failed with error\n");
            exit(8);
         }
    #else
         if (write(sockfd, msg, size) == -1) {
            perror("Writing failed with error");
            exit(8);
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

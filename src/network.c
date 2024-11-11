// Client-server api              //
// Network functions              //
// Version 0.5                    //
// Bachelor`s work project        //
// Technical University of Kosice //
// 10.11.2024                     //
// Nikita Kuropatkin              //

#include"network.h"

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


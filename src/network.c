// Client-server api              //
// Network functions              //
// Version 0.7.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 20.12.2024                     //
// Nikita Kuropatkin              //

#include <stdio.h>
#include "include/network.h"
#include "include/error.h" //All errors defined + function proto

//////////////////////////////////////////
/// Data Receiver ///
//////////////////////////////////////////
/*
The purpose of this function is to receive data over open sockets 
on Windows and Linux.  
It takes the following parameters:  
1. `sockfd` - the ID of the socket where the data will be received.  
2. `msg` - a buffer where the received message will be written.  
3. `size` - the size of the message.  
The program exits in case of an error.
*/
void read_win_lin(const int sockfd, uint8_t *msg, const uint32_t size)
{
 #ifdef _WIN32
    if (recv(sockfd, (char*)msg, size, 0) == SOCKET_ERROR) {
        exit_with_error(ERROR_RECEIVING_DATA, "Recieving failed");
    }
 #else
    if (read(sockfd, msg, size) == RETURN_ERROR) {
        exit_with_error(ERROR_RECEIVING_DATA, "Recieving failed");
    }
 #endif
}
//////////////////////////////////////////
//////////////////////////////////////////

//////////////////////////////////////////
/// Data Sender ///
//////////////////////////////////////////
/*
The purpose of this function is to send data over open sockets 
on Windows and Linux.  
It takes the following parameters:  
1. `sockfd` - the ID of the socket from which the data will be sent.  
2. `msg` - a buffer containing the message to be sent.  
3. `size` - the size of the message.  
The program exits in case of an error.
*/
void write_win_lin(const int sockfd, uint8_t *msg, const uint32_t size)
{
 #ifdef _WIN32
    if (send(sockfd, (char*)msg, size, 0) == SOCKET_ERROR) {
        exit_with_error(ERROR_SENDING_DATA, "Writing failed");
    }
 #else
    if (write(sockfd, msg, size) == RETURN_ERROR) {
        exit_with_error(ERROR_SENDING_DATA, "Writing failed");
    }
 #endif
}
//////////////////////////////////////////
//////////////////////////////////////////

//////////////////////////////////////////
/// Socket Closer ///
//////////////////////////////////////////
/*
The purpose of this function is to close existing sockets for 
Windows and Linux.  
It takes the following parameter:  
- `sockfd` - the ID of the socket to be closed.  
The program exits in case of an error.
*/
void sockct_cls(const int sockfd) {
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
This function purpose is to initialize socket for Windows,
program exits in case of error.
*/
void init_sock() {
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
/// Socket Creation Checker ///
///////////////////////////////
/*
The purpose of this function is to check if a socket was successfully created.  
It takes the following parameter:  
- `sockfd` - a variable containing the ID of the socket to be checked.  
The program exits in case of an error.
*/
void sock_check(const int sockfd) {
 if (sockfd == -1) {
    #ifdef _WIN32
        WSACleanup();
    #endif
    exit_with_error(ERROR_SOCKET_CREATION, "Socket creation failed");
 } 
 else {
    printf("Socket successfully created..\n");
 }
}
//////////////////////////////
//////////////////////////////


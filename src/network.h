// Client-server api              //
// Network functions              //
// Version 0.5.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 19.11.2024                     //
// Nikita Kuropatkin              //

/* 
This header file declares network functions
for a client-server application. Function definitions
are in network.c. 
*/
#ifndef NETWORK_H
#define NETWORK_H
#include <stdint.h>
//Windows Libs
#ifdef _WIN32
    #include <winsock2.h>   
    #include <ws2tcpip.h>   
    #include <io.h>
//Linux Libs
#else
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <netinet/in.h> 
    #include <sys/socket.h> 
    #include <sys/types.h>
#endif

#ifdef _WIN32// defining macro for address length(using different types in WIN and LINUX)
    #define LEN int 
#else
    #define LEN socklen_t
#endif 

//////////////////////////////////////////
/// Data receiver ///
//////////////////////////////////////////
/*
This function purpose is to receive data over open sockets for WIN and LIN OS
*/
void read_win_lin(int sockfd, uint8_t *msg, unsigned int size);
//////////////////////////////////////////
//////////////////////////////////////////

//////////////////////////////////////////
/// Data sender ///
//////////////////////////////////////////
/*
This function purpose is to send data over open sockets for WIN and LIN OS
*/
void write_win_lin(int sockfd, uint8_t *msg, unsigned int size);
//////////////////////////////////////////
//////////////////////////////////////////

//////////////////////////////////////////
/// Socket closer///
//////////////////////////////////////////
/*
This function purpose is to close existing sockets for WIN and LIN OS
*/
void sockct_cls(int sockfd);
///////////////////////////////////////////////////
///////////////////////////////////////////////////

///////////////////////////////
/// Socket initializer///
//////////////////////////////
/*
This function purpose is to initialize socket for windows
*/
void init_sock();
//////////////////////////////
//////////////////////////////

///////////////////////////////
/// Socket creation checker///
//////////////////////////////
/*
This function purpose is to check if socket was successfully created
*/
void sock_check(int sockfd);
//////////////////////////////
//////////////////////////////

#endif
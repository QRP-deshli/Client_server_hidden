// Client-server api              //
// Network functions              //
// Version 0.6                    //
// Bachelor`s work project        //
// Technical University of Kosice //
// 28.11.2024                     //
// Nikita Kuropatkin              //

/* 
This header file declares network TCP/IP functions
for a client-server application. Function bodies
are in network.c. 
*/
#ifndef NETWORK_H
#define NETWORK_H
#include <stdint.h>
/*Network libraries(will be used by client and server)*/
/*Windows Libs*/
#ifdef _WIN32
    #include <winsock2.h>   
    #include <ws2tcpip.h>   
    #include <io.h>
/*Linux Libs*/
#else
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <netinet/in.h> 
    #include <sys/socket.h> 
    #include <sys/types.h>
#endif

/*
Defining macro for address length
(using different types on WIN and LINUX)
*/
#ifdef _WIN32 
    #define LEN int 
#else
    #define LEN socklen_t
#endif 

//////////////////////////////////////////
/// Data receiver ///
//////////////////////////////////////////
/*
This function purpose is to receive data over open sockets for WIN and LIN OS,
program exits in case of error.
*/
void read_win_lin(int sockfd, uint8_t *msg, uint32_t size);
//////////////////////////////////////////
//////////////////////////////////////////

//////////////////////////////////////////
/// Data sender ///
//////////////////////////////////////////
/*
This function purpose is to send data over open sockets for WIN and LIN OS,
program exits in case of error.
*/
void write_win_lin(int sockfd, uint8_t *msg, uint32_t size);
//////////////////////////////////////////
//////////////////////////////////////////

//////////////////////////////////////////
/// Socket closer///
//////////////////////////////////////////
/*
This function purpose is to close existing sockets for WIN and LIN OS,
program exits in case of error.
*/
void sockct_cls(int sockfd);
///////////////////////////////////////////////////
///////////////////////////////////////////////////

///////////////////////////////
/// Socket initializer///
//////////////////////////////
/*
This function purpose is to initialize socket for windows,
program exits in case of error.
*/
void init_sock();
//////////////////////////////
//////////////////////////////

///////////////////////////////
/// Socket creation checker///
//////////////////////////////
/*
This function purpose is to check if socket was successfully created,
program exits in case of error.
*/
void sock_check(int sockfd);
//////////////////////////////
//////////////////////////////

#endif
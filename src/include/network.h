// Client-server api              //
// Network functions              //
// Version 0.7.0                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 16.12.2024                     //
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
    #include <unistd.h>
    #include <netinet/in.h> 
    #include <sys/socket.h> 
    #include <sys/types.h>
#endif

/*
Defining macro for address length
(it is needed cause it has different types on WIN and LINUX)
*/
#ifdef _WIN32 
    #define LEN int 
#else
    #define LEN socklen_t
#endif 

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
void read_win_lin(const int sockfd, uint8_t *msg, const uint32_t size);
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
void write_win_lin(const int sockfd, uint8_t *msg, const uint32_t size);
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

void sockct_cls(const int sockfd);
///////////////////////////////////////////////////
///////////////////////////////////////////////////

///////////////////////////////
/// Socket initializer///
//////////////////////////////
/*
This function purpose is to initialize socket for Windows,
program exits in case of error.
*/
void init_sock();
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
void sock_check(const int sockfd);
//////////////////////////////
//////////////////////////////

#endif

// Client-server api              //
// Error handling                 //
// Version 0.6.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 05.12.2024                     //
// Nikita Kuropatkin              //

/*
This header file declares macros for returning values
of error, also there is a function in config.c,
that ends program and returns error code value
(can be programmed to print error message)
*/
#ifndef ERROR_H
#define ERROR_H

//Defining errors with macros to use them in codes
#define OK 0
#define ERROR_WSA 1
#define ERROR_SOCKET_CREATION 2
#define ERROR_BINDING 3
#define ERROR_SOCKET_LISTENING 4
#define ERROR_SERVER_ACCEPT 5
#define ERROR_GETTING_INPUT 6
#define ERROR_RECEIVING_DATA 7  
#define ERROR_SENDING_DATA 8
#define ERROR_GENERATING_RANDOM 9
#define ERROR_CLIENT_CONNECTION 10
#define ERROR_PORT_INPUT 11
#define ERROR_IP_INPUT 12
#define MESSAGE_ALTERED 13
#define UNEQUAL_MAC 14
#define WRONG_PIN 15
#define WRONG_PIN_FORMAT 16
#define ALLOCATION_ERROR 17
#define TEXT_OVERFLOW 18
#define ERROR_OPENING_FILE 19
#define WRONG_TXT_FORMAT 20

//////////////////////////////////////////
/// Error printing ///
//////////////////////////////////////////
/*
This function purpose is to print errors in main code`s
You can choose where to print error stderr or stdout by switching 
macro OUT. Switch macro DEBUG to 0, to not print anything.
*/
void exit_with_error(const int error, const char * err_string);
//////////////////////////////////////////
//////////////////////////////////////////

#endif
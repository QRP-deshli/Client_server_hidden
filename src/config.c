// Client-server api              //
// Error handling                 //
// Version 0.7.0                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 16.12.2024                     //
// Nikita Kuropatkin              //

#include <stdio.h>
#include <stdlib.h>
#include "include/error.h"

//DEBUG macro options:
#define YES 1
#define NO 0
//OUT macro options:
#define OUT 1
#define ERR 0

#define PRINT OUT //prints to stdout, set to ERR to print to stderr
#define DEBUG NO //debug macro swith to YES to turn off printing errors 

//////////////////////////////////////////
/// Error printing ///
//////////////////////////////////////////
/*
The purpose of this function is to print errors defined by the variable 
error and the error message contained in the variable err_string. 
The function exits with the return value specified by error.
You can choose where to print the error message — either to stderr or stdout 
— by switching the PRINT macro. 
To suppress all output, set the DEBUG macro to YES.
*/
void exit_with_error(const int error, const char * err_string)
{
 if (PRINT == ERR && DEBUG == NO) {
    fprintf(stderr, "%s.\n", err_string);
    if(error != OK) {
        fprintf(stderr, "%s\n", "Error occured, program exited, press Enter:");
    }
    getchar();
 }
 else if (PRINT == OUT && DEBUG == NO) {
    fprintf(stdout, "%s.\n", err_string); 
    if (error != OK) {
        fprintf(stderr, "%s\n", "Error occured, program exited, press Enter:");
    }
    getchar();
    }
 exit(error);
}
//////////////////////////////////////////
//////////////////////////////////////////
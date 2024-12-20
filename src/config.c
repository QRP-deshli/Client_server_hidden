// Client-server api              //
// Error handling                 //
// Version 0.7.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 20.12.2024                     //
// Nikita Kuropatkin              //

#include <stdio.h>
#include <stdlib.h>
#include "include/error.h"
#include "include/parameters.h" //Macros are defined here

//////////////////////////////////////////
/// Error printing ///
//////////////////////////////////////////
/*
The purpose of this function is to print errors defined by the variable 
error and the error message contained in the variable err_string. 
The function exits with the return value specified by error.
This function takes the following parameters:  
- `err_string` - string with error message that will be printed.  
- `error` - an error code, that program will return, while exiting
   error codes are defined in error.h  
You can choose where to print the error message — either to stderr or stdout 
— by switching the PRINT macro. 
To suppress all output, set the DEBUG macro to YES.
Change macros in include/parameters.h
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
 else if (PRINT == OUTPUT && DEBUG == NO) {
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
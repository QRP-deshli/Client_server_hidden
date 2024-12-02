// Client-server api              //
// Error handling                 //
// Version 0.6                    //
// Bachelor`s work project        //
// Technical University of Kosice //
// 28.11.2024                     //
// Nikita Kuropatkin              //

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

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
This function purpose is to print errors in main code`s
You can choose where to print error stderr or stdout by switching 
macro PRINT. Switch macro DEBUG to YES, to not print anything.
*/
void exit_with_error(int error, char * err_string)
{
 if(PRINT == ERR && DEBUG == NO){
    fprintf(stderr, "%s.\n", err_string);
    if(error != OK)
        fprintf(stderr, "%s\n", "Error occured, program exited, press Enter:");
    getchar();
 }
 else if(PRINT == OUT && DEBUG == NO){
    fprintf(stdout, "%s.\n", err_string); 
    if(error != OK)
        fprintf(stderr, "%s\n", "Error occured, program exited, press Enter:");
    getchar();
    }
 exit(error);
}
//////////////////////////////////////////
//////////////////////////////////////////
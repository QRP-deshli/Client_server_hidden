#include"error.h"
#define OUT 1 //prints to stdout, set to 0 to print to stderr
#define DEBUG 0 //debug macro swith to 1 to turn off printing errors 

//////////////////////////////////////////
/// Error printing ///
//////////////////////////////////////////
/*
This function purpose is to print errors in main code`s
You can choose where to print error stderr or stdout by switching 
macro OUT.
*/
void exit_with_error(int error, char * err_string){
        if(OUT == 0 && DEBUG == 0){
            fprintf(stderr, "%s.\n", err_string);
        }
        else if(OUT == 1 && DEBUG == 0){
            fprintf(stdout, "%s.\n", err_string); 
        }
        exit(error);
}
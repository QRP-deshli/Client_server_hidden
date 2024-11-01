#include"error.h"
#define OUT 1 //prints to stdout, set to 0 to print to stderr
//Also you can switch it to antoher and it will not print at all

//////////////////////////////////////////
/// Error printing ///
//////////////////////////////////////////
/*
This function purpose is to print errors in main code`s
You can choose where to print error stderr or stdout by switching 
macro OUT.
*/
void exit_with_error(int error, char * err_string){
        if(OUT == 0){
            fprintf(stderr, "%s.\n", err_string);
        }
        else if(OUT == 1){
            fprintf(stdout, "%s.\n", err_string); 
        }
        exit(error);
}

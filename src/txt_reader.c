// Client-server api              //
// File reader                    //
// Version 0.7.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 20.12.2024                     //
// Nikita Kuropatkin              //

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "include/error.h" //All errors defined + function proto
#include "include/txt_reader.h"

////////////////////////
/// .txt File Reader ///
////////////////////////
/*
The purpose of this function is to read data (`uint8_t` arrays) from 
.txt files.  
It takes the following parameters:  
- `path` - the path to the .txt file relative to the working directory  
  (the directory where the .exe file is located).  
- `buffer` - an array where the read data (e.g., key, salt, pin arrays) 
  will be stored.  
- `size` - the size of the array to read from the file 
  (number of characters).  
*/
void read_from_txt(const char *path, uint8_t *buffer, const int size) {
 const char *filename = path; //Path to the text file
 FILE *file = fopen(filename, "r"); //Open file for reading only 

 if (!file) {
    printf("\nIn %s:\n", path);
    exit_with_error(ERROR_OPENING_FILE,"Error txt wasn`t found");
 }

 // Read key, pin or salt from txt
 for (int i = 0; i < size; i++) {
    /*Scaning elements of array and checking format(should be 0-255)*/
    if (fscanf(file, "%hhx", &buffer[i]) != 1) {
        printf("\nIn %s:\n", path);
        fclose(file);
        exit_with_error(WRONG_TXT_FORMAT,"File is wrongly formated");
    }
 }

 fclose(file);
}
///////////////////////
///////////////////////
// Client-server api              //
// File reader                    //
// Version 0.6.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 05.12.2024                     //
// Nikita Kuropatkin              //

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "error.h" //All errors defined + function proto
#include "txt_reader.h"

////////////////////////
/// .txt file reader ///
////////////////////////
/*
This function purpose is read data(uint8_t arrays) from .txt files.
Input variable "path" is a path to a file from working directory
(directory where .exe file is contained).
Variable "buffer" is an array where the read data will be written
(key, salt, pin arrays).
Var "size" is an size of array that we gonna read from file
(amount of chars)
*/
void read_from_txt(const char *path, uint8_t *buffer, const int size){
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
        exit_with_error(WRONG_TXT_FORMAT,"File is wrongly formated");
        fclose(file);
    }
 }

 fclose(file);
}
///////////////////////
///////////////////////
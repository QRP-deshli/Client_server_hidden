// Client-server api              //
// File reader                    //
// Version 0.6.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 05.12.2024                     //
// Nikita Kuropatkin              //

/* 
This header file declares file-reader function
for a client-server application. Function body
is in txt_reader.c. 
*/
#ifndef TXT_READER_H
#define TXT_READER_H
#include <stdint.h>

///////////////////////
/// TXT-file reader ///
///////////////////////
/*
This function purpose is read data(uint8_t arrays) from .txt files.
Input variable "path" is a path to a file from working directory
(directory where .exe file is contained).
Variable "buffer" is an array where the read data will be written
(key, salt, pin arrays).
Var "size" is an size of array that we gonna read from file
(amount of chars)
*/
void read_from_txt(const char *path, uint8_t *buffer, const int size);
///////////////////////
///////////////////////

#endif

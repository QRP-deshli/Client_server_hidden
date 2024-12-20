// Client-server api              //
// File reader                    //
// Version 0.7.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 20.12.2024                     //
// Nikita Kuropatkin              //

/* 
This header file declares file-reader function
for a client-server application. Function body
is in txt_reader.c. 
*/
#ifndef TXT_READER_H
#define TXT_READER_H
#include <stdint.h>

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
void read_from_txt(const char *path, uint8_t *buffer, const int size);
///////////////////////
///////////////////////

#endif

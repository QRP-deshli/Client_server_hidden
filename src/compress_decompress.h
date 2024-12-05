// Client-server api              //
// Compression functions          //
// Version 0.6.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 05.12.2024                     //
// Nikita Kuropatkin              //

/* 
This header file declares functions needed for 
compression/decompression using LZRW3a algorithm of users text for a 
client-server application. Function bodies
are in compress_decompress.c. 
LZRW3a: http://www.ross.net/compression/lzrw3a.html
*/
#ifndef COMPRESS_DECOMPRESS_H
#define COMPRESS_DECOMPRESS_H
#include <stdint.h>

/////////////////////////////////////////////
/// uint32_t to uint8_t array converter   ///
/////////////////////////////////////////////
/*
This functions takes as input uint32_t number
and pointer to uint8_t array(byte array). It
converts uint32_t number to byte array by bitwise
shifting number to right and logic &(AND) with 11111111, 
to get the right binary value in uint8_t array.
(We need this function to send size of compressed 
text to other size).
*/
void to_byte_array(const uint32_t number, uint8_t* byte_array);
/////////////////////////////////////////////
/////////////////////////////////////////////

/////////////////////////////////////////////
/// uint8_t array to uint32_t converter   ///
/////////////////////////////////////////////
/*
This functions takes as input uint32_t number
and pointer to uint8_t array(byte array). It
converts byte array to uint32_t number by bitwise
shifting byte array to left and logic |(OR) with 
uint32_t number, to get the right binary value 
in uint32_t number.
(We need this function to send size of compressed 
text to other size).
*/
uint32_t from_byte_array(const uint8_t* byte_array, uint32_t number);
/////////////////////////////////////////////
/////////////////////////////////////////////

/////////////////////////
/// Text compressors  ///
/////////////////////////
/*
This function takes as input uncompressed text (input_txt), 
the maximum size that the decompressed text can be (max_size), 
a pointer to the buffer where the compressed text 
will be stored (output_txt), and a pointer to its size (output_size).
Then, this function compresses input_txt using LZRW3a; 
the compressed text is written to output_txt.
If the compressed text is larger than output_txt, the program will exit 
(highly unlikely, as the buffer is 100 characters larger than input_txt). 
According to the LZRW3a documentation, the compressed text shouldn’t be 
drastically larger than the input text. Based on my experiments, text 
with extremely high entropy (45 unique characters) 
expanded by less than 10%; the output text was 49 characters long.
LZRW3-A: http://www.ross.net/compression/lzrw3a.html
*/
void compress_text(unsigned char *input_txt, const uint32_t max_size, unsigned  char *output_txt, uint32_t *output_size);
/////////////////////////
/////////////////////////

///////////////////////////
/// Text decompressors  ///
///////////////////////////
/*
This function takes as input compressed text (input_txt), 
its size (input_size), and a pointer to the buffer 
where the decompressed text will be stored (output_txt).
The function uses the LZRW3a algorithm for decompression.
LZRW3-A: http://www.ross.net/compression/lzrw3a.html
*/
void decompress_text(unsigned char *input_txt,unsigned char *output_txt, const uint32_t input_size);
///////////////////////////
///////////////////////////

#endif
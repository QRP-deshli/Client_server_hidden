// Client-server api              //
// Compression functions          //
// Version 0.6.5                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 05.12.2024                     //
// Nikita Kuropatkin              //

#include <string.h>
#include <stdlib.h>
#include "lzrw.h"
#include "error.h"

/*Macro function to allocate memory for LZRW3a work_area*/
#define ALLOCATE_WORK_AREA(size) malloc(size) 
/*Macro function to de-allocate work_area memory*/
#define FREE_WORK_AREA(ptr)      free(ptr) 

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
void to_byte_array(const uint32_t number, uint8_t* byte_array){
 byte_array[0] = (number >> 24) & 0xFF; 
 byte_array[1] = (number >> 16) & 0xFF;
 byte_array[2] = (number >> 8) & 0xFF;
 byte_array[3] = number & 0xFF; 
}
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
uint32_t from_byte_array( const uint8_t* byte_array, uint32_t number){
 number |= (uint32_t)byte_array[0] << 24;
 number |= (uint32_t)byte_array[1] << 16;
 number |= (uint32_t)byte_array[2] << 8;
 number |= (uint32_t)byte_array[3];
 return number;
}
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
void compress_text(unsigned char *input_txt, const uint32_t max_size, unsigned  char *output_txt, uint32_t *output_size)
{
 /*Working memory for LZRW3a compression*/
 void *wrk_mem = ALLOCATE_WORK_AREA(lzrw3a_req_mem()); 
 if (wrk_mem == NULL) {
    exit_with_error(ALLOCATION_ERROR,"Memory allocation failed");
 }
    
 /*LZRW3a compress*/ 
 lzrw3a_compress(COMPRESS_ACTION_COMPRESS,wrk_mem,input_txt, strlen((const char *)input_txt),output_txt,output_size); 

 FREE_WORK_AREA(wrk_mem);

 if(*output_size>max_size)
 {
	exit_with_error(TEXT_OVERFLOW, "Compressed text size is bigger than buffer");
 }
}
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
void decompress_text(unsigned char *input_txt,unsigned char *output_txt, const uint32_t input_size)
{
 /*Output size(should be for algorithm, but actually unused in program)*/
 uint32_t output_size = 0; 
 /*Working memory for LZRW3a decompression*/
 void *wrk_mem = ALLOCATE_WORK_AREA(lzrw3a_req_mem()); 

 /*LZRW3a decompress*/
 lzrw3a_compress(COMPRESS_ACTION_DECOMPRESS,wrk_mem,input_txt,input_size,output_txt,&output_size); 
 FREE_WORK_AREA(wrk_mem);
}
///////////////////////////
///////////////////////////
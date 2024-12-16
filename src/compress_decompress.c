// Client-server api              //
// Compression functions          //
// Version 0.7.0                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 16.12.2024                     //
// Nikita Kuropatkin              //

#include <string.h>
#include <stdlib.h>
#include "include/lzrw.h"
#include "include/error.h"

/*!DEMO!*/
/* Macros for deciding the type of allocation */
#define STATIC 0
#define DYNAMIC 1
#define ALLOCATION DYNAMIC  // Set ALLOCATION to STATIC or DYNAMIC

/* Macro function to allocate memory for LZRW3a work_area */
#define ALLOCATE_WORK_AREA(size) malloc(size) 

/*
 Macro function to de-allocate work_area memory.
 If ALLOCATION is set to STATIC, does nothing.
 If ALLOCATION is set to DYNAMIC, frees the allocated memory.
*/
#if ALLOCATION == DYNAMIC
  #define FREE_WORK_AREA(ptr) free(ptr)
#else
  #define FREE_WORK_AREA(ptr)  // No action for STATIC allocation
#endif


/////////////////////////////////////////////
/// uint32_t to uint8_t Array Converter   ///
/////////////////////////////////////////////
/*
This function takes the following parameters:  
- `number` - a `uint32_t` number that will be converted.  
- `byte_array` - a pointer to a `uint8_t` array (byte array) where the 
  converted value will be stored.  
The function converts the `uint32_t` number to a byte array and writes 
it in Big Endian format.  
This function is needed to send the size of the compressed text to the 
other side.
*/
void to_byte_array(const uint32_t number, uint8_t* byte_array) {
 byte_array[0] = (number >> 24) & 0xFF; 
 byte_array[1] = (number >> 16) & 0xFF;
 byte_array[2] = (number >> 8) & 0xFF;
 byte_array[3] = number & 0xFF; 
}
/////////////////////////////////////////////
/////////////////////////////////////////////

/////////////////////////////////////////////
/// uint8_t Array to uint32_t Converter   ///
/////////////////////////////////////////////
/*
This function takes the following parameters:  
- `number` - a `uint32_t` variable where the converted value will be stored.  
- `byte_array` - a pointer to a `uint8_t` array (byte array) 
  that is in Big Endian format.  
The function converts the Big Endian byte array to a `uint32_t` number.  
This function is needed to send the size of the compressed text 
to another side.
*/
uint32_t from_byte_array( const uint8_t* byte_array, uint32_t number) {
 number |= (uint32_t)byte_array[0] << 24;
 number |= (uint32_t)byte_array[1] << 16;
 number |= (uint32_t)byte_array[2] << 8;
 number |= (uint32_t)byte_array[3];
 return number;
}
/////////////////////////////////////////////
/////////////////////////////////////////////

/////////////////////////
/// Text Compressors  ///
/////////////////////////
/*
This function takes the following parameters:  
- `input_txt` - the uncompressed text to be compressed.  
- `max_size` - the maximum size that the compressed text can have.  
- `output_txt` - a pointer to the buffer where the compressed text 
  will be stored.  
- `output_size` - a pointer to the size of the compressed text.  
The function compresses `input_txt` using the LZRW3a algorithm,  
and the compressed text is written to `output_txt`.  

If the compressed text exceeds the size of `output_txt`, the program 
will exit. This is highly unlikely, as the buffer is 100 characters larger 
than `input_txt`. According to the LZRW3a documentation, the compressed text 
should not be drastically larger than the input text. Based on experiments, 
text with extremely high entropy (45 unique characters) expanded by less 
than 10%, with the output text being 49 characters long.  
For more information on LZRW3a, see:  
LZRW3-A: http://www.ross.net/compression/lzrw3a.html
*/
void compress_text(unsigned char *input_txt, const uint32_t max_size, unsigned  char *output_txt, uint32_t *output_size)
{
 /*
  Working memory for LZRW3a compression.
  The allocation type is determined based on the ALLOCATION flag.
  In future versions, this should be refactored to allow decisions at 
  preprocessing time.
 */
 #if ALLOCATION == DYNAMIC
   void *wrk_mem = ALLOCATE_WORK_AREA(lzrw3a_req_mem());
   if (wrk_mem == NULL) {
       exit_with_error(ALLOCATION_ERROR, "Memory allocation failed");
   }
 #else
   static uint8_t wrk_mem[MEM_REQ];
 #endif

 /*LZRW3a compress*/ 
 lzrw3a_compress(COMPRESS_ACTION_COMPRESS,wrk_mem,input_txt, strlen((const char *)input_txt),output_txt,output_size); 

 FREE_WORK_AREA(wrk_mem);

 if (*output_size>max_size) {
	exit_with_error(TEXT_OVERFLOW, "Compressed text size is bigger than buffer");
 }
}
/////////////////////////
/////////////////////////

///////////////////////////
/// Text Decompressors  ///
///////////////////////////
/*
This function takes the following parameters:  
- `input_txt` - the compressed text to be decompressed.  
- `max_size` - the maximum size that the decompressed text can have.  
- `input_size` - the size of the compressed text.  
- `output_txt` - a pointer to the buffer where the decompressed 
  text will be stored.  
The function decompresses `input_txt` using the LZRW3a algorithm,  
and the decompressed text is written to `output_txt`.  
For more information on LZRW3a, see:  
LZRW3-A: http://www.ross.net/compression/lzrw3a.html
*/
void decompress_text(unsigned char *input_txt, const uint32_t max_size, unsigned char *output_txt, const uint32_t input_size)
{
 /*Output size*/
 uint32_t output_size = 0; 
 /*
  Working memory for LZRW3a compression.
  The allocation type is determined based on the ALLOCATION flag.
  In future versions, this should be refactored to allow decisions at 
  preprocessing time.
 */
 #if ALLOCATION == DYNAMIC
   void *wrk_mem = ALLOCATE_WORK_AREA(lzrw3a_req_mem());
   if (wrk_mem == NULL) {
       exit_with_error(ALLOCATION_ERROR, "Memory allocation failed");
   }
 #else
   static uint8_t wrk_mem[MEM_REQ];
 #endif

 /*LZRW3a decompress*/
 lzrw3a_compress(COMPRESS_ACTION_DECOMPRESS,wrk_mem,input_txt,input_size,output_txt,&output_size); 
 FREE_WORK_AREA(wrk_mem);

 if (output_size>max_size) {
   exit_with_error(TEXT_OVERFLOW, "Compressed text size is bigger than buffer");
 }
}
///////////////////////////
///////////////////////////
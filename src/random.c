// Client-server api              //
// Random number generator        //
// Version 0.5                    //
// Bachelor`s work project        //
// Technical University of Kosice //
// 10.11.2024                     //
// Nikita Kuropatkin              //
#include"random.h"

//////////////////////////////////////////
/// Random numbers generator ///
//////////////////////////////////////////
/*
This function purpose is to generate random bits and fill received array with them.
This function works for Win and Lin.
*/
void random_num(uint8_t * number, int size){
        #ifdef _WIN32
            unsigned int temp;
            for(int i = 0;i<size;i++){
                if (rand_s(&temp) != 0) {
                exit_with_error(ERROR_GENERATING_RANDOM, "Number generation failed");
                }
                number[i] = (uint8_t)temp;
            }  
        #else
            arc4random_buf(number, size);
            if (number == NULL) {
                exit_with_error(ERROR_GENERATING_RANDOM, "Number generation failed");
            }
        #endif
}
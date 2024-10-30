#include"shared.h"


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
                printf("Number generation failed\n");
                exit(9);
                }
                number[i] = (uint8_t)temp;
            }  
        #else
            arc4random_buf(number, size);
            if (number == NULL) {
                printf("Number generation failed\n");
                exit(9);
            }
        #endif

}

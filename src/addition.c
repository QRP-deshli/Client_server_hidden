// Client-server api              //
// Additional functions           //
// Version 0.5                    //
// Bachelor`s work project        //
// Technical University of Kosice //
// 10.11.2024                     //
// Nikita Kuropatkin              //

#include"addition.h"

////////////////////////
/// Clearing input   ///
////////////////////////
/*This function purpose is to clear stdin after 
entering oversized message(stdin overflow)*/
void clear (){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
////////////////////////
////////////////////////

///////////////////////////
/// Exit on stop-word   ///
///////////////////////////
/*This function purpose is to stop program
after detecting stop-word in message side 
is just a modifier for a message, that will
tell who ended communication*/
int exiting (char *side, char *msg){
    // Exit the loop if the message contains stop-word
    if (strncmp(msg,EXIT,strlen(EXIT)) == 0) {
        printf("%s exited...\n",side);
        return 1;
    }
    else return 0;
}
///////////////////////////
///////////////////////////


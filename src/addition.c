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

///////////////////////////
/// Checking input IP   ///
///////////////////////////
/*This function purpose is to check ,whether
user entered valid IP-address*/
void ip_check (char *ip){
    int test;
    int dot_count = 0;
    char *copyip = calloc(strlen(ip)+1, sizeof(char));//copy of the string 
    strcpy(copyip,ip);

    for(int i = 0;i<(int)strlen(ip);i++){//checking amount of dots in ipv4
        if(copyip[i] == '.')dot_count++;
    }
    if(dot_count != 3)exit_with_error(ERROR_IP_INPUT,"Invalid IP");//Checking IP

    char *ptr = strtok(copyip, ".");//divide string to tokens
    test = atoi(ptr);//one quarter of ipv4 to a number
    if(test<0 || test>255)exit_with_error(ERROR_IP_INPUT,"Invalid IP");//Checking quarter of IP
    while (ptr != NULL){
        ptr = strtok(NULL, ".");
        test = atoi(ptr); //one quarter of ipv4 to a number
        if(test<0 || test>255)exit_with_error(ERROR_IP_INPUT,"Invalid IP");//Checking quarter of IP
    }
}
///////////////////////////
///////////////////////////

////////////////////////////////
/// Printing help for user   ///
////////////////////////////////
/*This function purpose is to print basic info 
for user and then end the program.
The parameter(int side) is a switch to print specific 
info for a specific side(server and client),basically it 
defines what side called this function.
Other parameters represent default values of port, message size and IP*/
void help_print (int side, int port, char *ip, int max){
    printf("\n");
    printf("************************************************************************\n");
    if(side == 1)printf("# You are trying to execute client side #\n");
    else printf("# You are trying to execute server side #\n");
    printf("# If you simply execute it, it will use default parameters #\n");
    printf("# Default port number(must be same on both sides): %d #\n",port);
    if(side == 1)printf("# Default IP address of server: %s #\n",ip);
    printf("# You can also change it by executing program with arguments: #\n");
    if(side == 1){
        printf("#./client.exe <PORT> <IP> #\n");
        printf("#Example: ./client.exe 8888 192.168.1.1 #\n");
    }
    if(side == 0){
        printf("#./server.exe <PORT> #\n");
        printf("#Example: ./client.exe 8888 #\n");
    }
    printf("#Also both sides have a limited input text size of %d symbols #\n",max);
    printf("************************************************************************\n");
    printf("\n");
    exit_with_error(OK,"You can re-run program now");
}
////////////////////////////////
////////////////////////////////



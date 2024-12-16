// Client-server api              //
// Additional functions           //
// Version 0.7.0                  //
// Bachelor`s work project        //
// Technical University of Kosice //
// 16.12.2024                     //
// Nikita Kuropatkin              //

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/addition.h"
#include "include/error.h"
#include "include/macros.h"

/*Stop-word, if someone use it in conversation, it will end*/
#define EXIT "exit"

/*
 Two next macros define allowed range 
 of values for octet in IPV4(0.0.0.0 - 255.255.255.255)
*/ 
#define IPSTART 0
#define IPEND 255

////////////////////////
/// Clearing Input   ///
////////////////////////
/*
The purpose of this function is to clear the `stdin` buffer after  
an oversized message is entered (to handle `stdin` overflow).
*/
void clear (void)
{
 int c;
 while ((c = getchar()) != '\n' && c != EOF);
}
////////////////////////
////////////////////////

///////////////////////////
/// Exit on Stop-Word   ///
///////////////////////////
/*
The purpose of this function is to terminate the program  
when a stop-word is detected in the message.  
- The `side` variable is a modifier for the message that indicates  
  who ended the communication.  
- The stop-word can be configured by changing the `EXIT` macro.  
The function returns a value of 1 if the stop-word is detected.  
After returning, the program (server or client) proceeds with  
closing the communication session as normal.
*/
int exiting (const char *side, const char *msg)
{
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
/// Checking Input IP   ///
///////////////////////////
/*
The purpose of this function is to check whether the  
user has entered a valid IP address in the correct format.  
It takes the following parameter:  
- `ip` - contains the IP address to be validated.  
If the entered IP address is not valid (not correctly formatted),  
the program exits.  

Valid format examples:  
- Dotted decimal format: 192.168.0.1  
- Each octet must have a maximum value of 255: 255.255.255.255  
  (the "maximum" IP address).
*/
void ip_check (char *ip)
{
 int test;
 int dot_count = 0;

 /*Checking dotted decimal form:*/
 for (int i = 0;i<(int)strlen(ip);i++) { //Checking amount of dots in ipv4
    if (ip[i] == '.') dot_count++;
 }
 /*If amount of dots is less than 3 - exits*/
 if (dot_count != 3) exit_with_error(ERROR_IP_INPUT,"Invalid IP"); 

 /*Checking maximal value of octets:*/
 char *ptr = strtok(ip, "."); //Divide string to tokens
 test = atoi(ptr); //One quarter of ipv4 to a number

 /*Checking first octet of IP*/
 if (test<IPSTART || test>IPEND) exit_with_error(ERROR_IP_INPUT,"Invalid IP"); 
 while (ptr != NULL) {
    ptr = strtok(NULL, ".");
    test = atoi(ptr); //Convert one octet of ipv4 to a decimal form
    /*Checking other octets of IP*/
    if (test<IPSTART || test>IPEND) exit_with_error(ERROR_IP_INPUT,"Invalid IP"); 
 }
}
///////////////////////////
///////////////////////////

////////////////////////////////
/// Printing Help for User   ///
////////////////////////////////
/*
The purpose of this function is to print basic information  
for the user and then terminate the program.  
The parameter `side` is a switch used to print  
specific information for either the server or the client,  
indicating which side called this function.  
Other parameters:  
- `port` - the default port value.  
- `ip` - the default IP address.  
- `max` - the default message size.
*/
void help_print (const int side, const int port, const char *ip, const int max)
{
 printf("\n");
 printf("***********************************************************************\n");
 if (side == CLIENT) printf("# You are trying to execute client side #\n");
 else printf("# You are trying to execute server side #\n");
 printf("# If you simply execute it, it will use default parameters #\n");
 printf("# Default port number(must be same on both sides): %d #\n",port);
 if (side == CLIENT) printf("# Default IP address of server: %s #\n",ip);
 printf("# You can also change it by executing program with arguments:#\n");
 if (side == CLIENT) {
    printf("#./client.exe <PORT> <IP> #\n");
    printf("#Example: ./client.exe 8888 192.168.1.1 #\n");
 }
 if (side == SERVER) {
    printf("#./server.exe <PORT> #\n");
    printf("#Example: ./client.exe 8888 #\n");
 }
 printf("#Also both sides have a limited input text size of %d symbols #\n",max);
 printf("************************************************************************\n");
 printf("\n");
 exit_with_error(OK,"You can re-run program now"); //Normal exit
}
////////////////////////////////
////////////////////////////////




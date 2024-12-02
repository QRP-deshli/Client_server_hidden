// Client-server api              //
// Additional functions           //
// Version 0.6                    //
// Bachelor`s work project        //
// Technical University of Kosice //
// 28.11.2024                     //
// Nikita Kuropatkin              //

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "addition.h"
#include "error.h"

/*Stop-word, if someone use it in conversation, it will end*/
#define EXIT "exit" 

/*
Two next Macros are used for easier understanding of code,
some functions have different function order or print different
strings depending on which side called it(Server or Client)
*/
#define SERVER 0 // Macro that defines Server side
#define CLIENT 1 // Macro that defines Client side

////////////////////////
/// Clearing input   ///
////////////////////////
/*This function purpose is to clear stdin after 
entering oversized message(stdin overflow)*/
void clear (void)
{
 int c;
 while ((c = getchar()) != '\n' && c != EOF);
}
////////////////////////
////////////////////////

///////////////////////////
/// Exit on stop-word   ///
///////////////////////////
/*This function purpose is to stop program
after detecting stop-word in message,
variable side is just a modifier for a message, that will
tell who ended communication. You can configure stop-word, 
by changing macro EXIT.
Returns value 1 if the word was used, after that
program(server or client) proceeds in normal
closing communication session
*/
int exiting (char *side, char *msg)
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
/// Checking input IP   ///
///////////////////////////
/*This function purpose is to check, whether
user entered valid IP-address(should be in formated right). 
If inputed IP isn`t valid(isn`t formated right) - program exits
Right format examples:
# Dotted decimal form: 192.168.0.1
# Each octet MAX value is 255: 255.255.255.255 -- "maximum" ip address
*/
void ip_check (char *ip)
{
 int test;
 int dot_count = 0;

 /*Checking dotted decimal form:*/
 for(int i = 0;i<(int)strlen(ip);i++){ //Checking amount of dots in ipv4
    if(ip[i] == '.')dot_count++;
 }
 /*If amount of dots is less than 3 - exits*/
 if(dot_count != 3)exit_with_error(ERROR_IP_INPUT,"Invalid IP"); 

 /*Checking maximal value of octets:*/
 char *ptr = strtok(ip, "."); //Divide string to tokens
 test = atoi(ptr); //One quarter of ipv4 to a number

 /*Checking first octet of IP*/
 if(test<0 || test>255)exit_with_error(ERROR_IP_INPUT,"Invalid IP"); 
 while (ptr != NULL){
    ptr = strtok(NULL, ".");
    test = atoi(ptr); //Convert one octet of ipv4 to a decimal form
    /*Checking other octets of IP*/
    if(test<0 || test>255)exit_with_error(ERROR_IP_INPUT,"Invalid IP"); 
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
info for a specific side(server and client), basically it 
defines what side called this function.
Other parameters represent default values of port, message size and IP*/
void help_print (int side, int port, char *ip, int max)
{
 printf("\n");
 printf("***********************************************************************\n");
 if(side == CLIENT)printf("# You are trying to execute client side #\n");
 else printf("# You are trying to execute server side #\n");
 printf("# If you simply execute it, it will use default parameters #\n");
 printf("# Default port number(must be same on both sides): %d #\n",port);
 if(side == CLIENT)printf("# Default IP address of server: %s #\n",ip);
 printf("# You can also change it by executing program with arguments:#\n");
 if(side == CLIENT){
    printf("#./client.exe <PORT> <IP> #\n");
    printf("#Example: ./client.exe 8888 192.168.1.1 #\n");
 }
 if(side == SERVER){
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




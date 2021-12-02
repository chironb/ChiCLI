// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2020 by: Chiron Bramberger
//
// ********************************************************************************

// ********************************************************************************
// alias.c 
// ********************************************************************************

#include <stdio.h>
#include <string.h>

#ifndef ALIAS_H
#define ALIAS_H
#include "alias.h"
#endif 

#ifndef CHICLI_H
#define CHICLI_H
#include "chicli.h"
#endif 

#ifndef STRING_PROCESSING_H
#define STRING_PROCESSING_H
#include "string_processing.h"
#endif 

extern unsigned char alias_shortcut_list[MAX_ALIASES][MAX_ALIAS_LENGTH] ;
extern unsigned char alias_command_list[MAX_ALIASES][MAX_ALIAS_LENGTH] ;

// ********************************************************************************
// ALIAS FUNCTIONS 
// ********************************************************************************

unsigned char do_alias( unsigned char *input_string ) {

	unsigned char i = 0;	

	if (input_string[0] == 'a' && 
		input_string[1] == 'l' && 
		input_string[2] == 'i' && 
		input_string[3] == 'a' && 
		input_string[4] == 's'    ){

			return(2); // skipped because we dont' want to alias an alias command itself - our heads would explode and smoke would come out of the computer like an original series Star Trek episode!!!

	} else {

		for (i = 0 ; i < MAX_ALIASES ; i++) {
			if ( strcmp(input_string, alias_shortcut_list[i]) == 0 ) {
				strncpy(input_string, alias_command_list[i],MAX_ALIAS_LENGTH-1);
				replace_characters(input_string, '\"', ' ');
				replace_characters(input_string, ';', '\"');
				return(1); // success! found it!
			};//end if 
		}//end for 

	};//end if 

	return(0); // fail! couldn't find it!

};//end func 


unsigned char clear_alias( unsigned char *input_string ) {

	unsigned char i = 0;

	for (i = 0 ; i < MAX_ALIASES ; i++) {

		if ( strcmp(input_string, alias_shortcut_list[i]) == 0 ) {
			strcpy(alias_shortcut_list[i], "\0");
			strcpy(alias_command_list[i], "\0");
			return(1); 												// success! found it! cleared it!
		};//end if 

	}//end for 

	return(0); 														// fail! couldn't find it!

};//end func 


void clear_alias_all( void ) {

	unsigned char i = 0;

	for (i = 0 ; i < MAX_ALIASES ; i++) {
		strcpy(alias_shortcut_list[i], "\0");
		strcpy(alias_command_list[i], "\0");
	}//end for 

};//end func 


unsigned char set_alias( unsigned char *input_string , unsigned char *actual_command) {

	unsigned char i = 0;

	for (i = 0 ; i < MAX_ALIASES ; i++) {

		if ( strcmp(alias_shortcut_list[i], "\0") == 0 || (strcmp(alias_shortcut_list[i], input_string) == 0) ) { 			//find a blank one
			strncpy(alias_shortcut_list[i],input_string, MAX_ALIAS_LENGTH-1);
			strncpy(alias_command_list[i], actual_command, MAX_ALIAS_LENGTH-1);
			return(1); 												// success! found it!
		};//end if 

	}//end for 

	return(0); 														// fail! couldn't find it!

};//end func 


void display_alias_all( void ) {

	unsigned char i = 0;
	unsigned char displayed_aliases = 0;

	for (i = 0 ; i < MAX_ALIASES ; i++) {
		if ( strcmp(alias_shortcut_list[i], "\0") != 0 ) { //find a blank one
			printf("%s = %s\n",alias_shortcut_list[i], alias_command_list[i]);	
			displayed_aliases++;
		};//end if 	
	}//end for 

	if (displayed_aliases == 0) {
		printf("Slots empty.\n");
	};//end if 

	printf("Maximum alias length:%u\nTotal alias slots:%u\n",MAX_ALIAS_LENGTH,MAX_ALIASES);

};//end func 
